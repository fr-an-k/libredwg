/*****************************************************************************/
/*  LibreDWG - free implementation of the DWG file format                    */
/*                                                                           */
/*  Copyright (C) 2018-2023 Free Software Foundation, Inc.                   */
/*                                                                           */
/*  This library is free software, licensed under the terms of the GNU       */
/*  General Public License as published by the Free Software Foundation,     */
/*  either version 3 of the License, or (at your option) any later version.  */
/*  You should have received a copy of the GNU General Public License        */
/*  along with this program.  If not, see <http://www.gnu.org/licenses/>.    */
/*****************************************************************************/

/* convert.c: convert any to any format (multiple output support).
 * Initially intended for WASM/js; a main() could be created later,
 * possibly consolidating existing programs.
 * 
 * written by Frank van Leeuwen
 */

#include "dwg.h"
#include "decode.h"
#ifndef DISABLE_JSON
#  include "in_json.h"
#  include "out_json.h"
#endif
#include "in_dxf.h"
#include "out_dxf.h"

// Note: inputs are sanitized in js
void convert(
    unsigned char* in, size_t size, const char* version, const char* type,
    unsigned char* dxf, size_t dxf_size, const char* dxf_version,
    unsigned char* dxfb, size_t dxfb_size, const char* dxfb_version,
    unsigned char* json, size_t json_size) {

  Dwg_Data dwg;
  Bit_Chain dat = { 0 };
  int error = 0;

  memset (&dwg, 0, sizeof (Dwg_Data));
  //dwg.opts = opts;
  if (version) // hint the importer
    dwg.header.version = dat.version = version;

  dat.chain = in;
  if (!strcmp(type, "dwg")) {
    error = dwg_decode (&dat, &dwg);
  }
  else if (!strcmp(type, "dxf")) {
    error = dwg_read_dxf (&dat, &dwg);
  }
  else if (!strcmp(type, "dxfb")) {
    error = dwg_read_dxfb (&dat, &dwg);
  }
  else if (!strcmp(type, "json")) {
    error = dwg_read_json (&dat, &dwg);
  }
  if (error)
    return error;

  if (dxf && dxf_size) {
    error = dwg_write_dxf(&dat, &dwg);
    if (error)
      return error;
  }
  if (dxfb && dxfb_size) {
    error = dwg_write_dxfb(&dat, &dwg);
    if (error)
      return error;
  }
  if (json && json_size) {
    error = dwg_write_json(&dat, &dwg);
    if (error)
      return error;
  }

  return error;
}