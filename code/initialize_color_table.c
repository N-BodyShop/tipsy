/*
 * $Header$
 * $Log$
 * Revision 1.2.6.1  2000/06/22 21:14:46  nsk
 * Added red temperature table. (maf)
 *
 * Revision 1.2  1995/03/24  18:49:08  trq
 * Added "revrain" colormap.
 *
 * divv.c: included malloc.h.
 *
 * Revision 1.1.1.1  1995/01/10  22:57:25  trq
 * Import to CVS
 *
 * Revision 2.3  94/03/24  11:49:52  nsk
 * made dark particles lighter
 * 
 * Revision 2.2  94/03/24  11:09:05  trq
 * Fixed typo in "puke" colormap.
 * 
 * Revision 2.1  94/02/16  10:29:25  trq
 * Added "puke" colors to "all" colormap.
 * 
 */
#include "defs.h"

void
initialize_color_table()
{

    int i,j ;
    double slope ;
    double offset ;
 
    slope = 205./42. ;
    for(i = 0 ;i < 43 ;i++){
	rainbow_red[i] = 255 ;
	rainbow_green[i] = (int)(slope * (double)i + 50. + .5) ;
	rainbow_blue[i] = 0 ;
    }
    slope = 205./21. ;
    for(i = 43 ;i < 64 ;i++){
	rainbow_red[i] = 255 - (int)(slope * (double)(i - 42) + .5) ;
	rainbow_green[i] = 255 ;
	rainbow_blue[i] = 0 ;
    }
    slope = 205./29. ;
    for(i = 64 ;i < 94 ;i++){
	rainbow_red[i] = 0 ;
	rainbow_green[i] = 255 ;
	rainbow_blue[i] = (int)(slope * (double)(i - 64) + 50. + .5) ;
    }
    slope = 255./31. ;
    for(i = 94 ;i < 125 ;i++){
	rainbow_red[i] = 0 ;
	rainbow_green[i] = 255 - (int)(slope * (double)(i - 93) + .5) ;
	rainbow_blue[i] = 255 ;
    }

    slope = 255./31. ;
    for(i = 0 ;i < 31 ;i++){
	revrain_red[i] = 0 ;
	revrain_green[i] = (int)(slope * (double)(i) + .5) ;
	revrain_blue[i] = 255 ;
    }
    slope = 205./29. ;
    for(i = 31 ;i < 60 ;i++){
	revrain_red[i] = 0 ;
	revrain_green[i] = 255 ;
	revrain_blue[i] = 255 - (int)(slope * (double)(i - 31) + .5) ;
    }
    slope = 205./21. ;
    for(i = 60 ;i < 81 ;i++){
	revrain_red[i] = (int)(slope * (double)(i - 60) + 50.0 + .5) ;
	revrain_green[i] = 255 ;
	revrain_blue[i] = 0 ;
    }
    slope = 205./42. ;
    for(i = 81 ;i < 125 ;i++){
	revrain_red[i] = 255 ;
	revrain_green[i] = 255 - (int)(slope * (double)(i - 81)  + .5) ;
	revrain_blue[i] = 0 ;
    }

    slope = 255./20. ;
    for(i = 0 ;i < 21 ;i++){
	wrbb_red[i] = 0 ;
	wrbb_green[i] = 0 ;
	wrbb_blue[i] = (int)(slope * (double)(i) + .5) ;
    }
    slope = 191./20. ;
    offset = 64. - slope * 21. ;
    for(i = 21 ;i < 42 ;i++){
	wrbb_red[i] = (int)(slope * (double)(i) + offset + .5) ;
	wrbb_green[i] = 0 ;
	wrbb_blue[i] = 255 ;
    }
    slope = -205./21. ;
    offset = 255. - slope * 41. ;
    for(i = 42 ;i < 63 ;i++){
	wrbb_red[i] = 255 ;
	wrbb_green[i] = 0 ;
	wrbb_blue[i] = (int)(slope * (double)(i) + offset + .5) ;
    }
    slope = 205./40. ;
    offset = 50. - slope * 63. ;
    for(i = 63 ;i < 104 ;i++){
	wrbb_red[i] = 255 ;
	wrbb_green[i] = (int)(slope * (double)(i) + offset + .5) ;
	wrbb_blue[i] = 0 ;
    }
    slope = 255./21. ;
    offset = -slope * 103. ;
    for(i = 104 ;i < 125 ;i++){
	wrbb_red[i] = 255 ;
	wrbb_green[i] = 255 ;
	wrbb_blue[i] = (int)(slope * (double)(i) + offset + .5) ;
    }


    slope = 255./86. ;
    for (i=0; i<125; i++) {
      j = (int)(slope * (double)i );
      if (j > 255) j = 255;
      temp_red[i] = j;
    }
    slope = 255./66. ;
    for (i=0; i<125; i++) {
      j = (int)(slope * (double)(i - 58.) );
      if (j > 255) j = 255;
      if (j < 0) j = 0;
      temp_green[i] = j;
    }
    slope = 255./32. ;
    for (i=0; i<125; i++) {
      j = (int)(slope * (double)(i - 92.) );
      if (j > 255) j = 255;
      if (j < 0) j = 0;
      temp_blue[i] = j;
    }

    for (i = 125, j = 0 ; j < 125 ;i++, j++) {
	temp_red[i] = temp_red[j] ;
	temp_green[i] = temp_green[j] ;
	temp_blue[i] = temp_blue[j] ;
	wrbb_red[i] = wrbb_red[j] ;
	wrbb_green[i] = wrbb_green[j] ;
	wrbb_blue[i] = wrbb_blue[j] ;
	rainbow_red[i] = rainbow_red[j] ;
	rainbow_green[i] = rainbow_green[j] ;
	rainbow_blue[i] = rainbow_blue[j] ;
	revrain_red[i] = revrain_red[j] ;
	revrain_green[i] = revrain_green[j] ;
	revrain_blue[i] = revrain_blue[j] ;
    }
    colormap_red[0] = 0 ;
    colormap_green[0] = 0 ;
    colormap_blue[0] = 0 ;

    colormap_red[1] = 0 ;
    colormap_green[1] = 255 ;
    colormap_blue[1] = 0 ;

    colormap_red[2] = 190 ;
    colormap_green[2] = 200 ;
    colormap_blue[2] = 255 ;

    colormap_red[3] = 255 ;
    colormap_green[3] = 63 ;
    colormap_blue[3] = 63 ;

    colormap_red[4] = 255 ;
    colormap_green[4] = 255 ;
    colormap_blue[4] = 140 ;

    colormap_red[5] = 127 ;
    colormap_green[5] = 255 ;
    colormap_blue[5] = 212 ;

    colormap_red[6] = 255 ;
    colormap_green[6] = 160 ;
    colormap_blue[6] = 122 ;

    colormap_red[7] = 238 ;
    colormap_green[7] = 224 ;
    colormap_blue[7] = 229 ;

    colormap_red[8] = 255 ;
    colormap_green[8] = 239 ;
    colormap_blue[8] = 213 ;

    colormap_red[9] = 238 ;
    colormap_green[9] = 233 ;
    colormap_blue[9] = 191 ;

    colormap_red[10] = 72 ;
    colormap_green[10] = 118 ;
    colormap_blue[10] = 255 ;

    colormap_red[11] = 192 ;
    colormap_green[11] = 192 ;
    colormap_blue[11] = 192 ;

    colormap_red[12] = 255 ;
    colormap_green[12] = 231 ;
    colormap_blue[12] = 186 ;

    colormap_red[13] = 0 ;
    colormap_green[13] = 154 ;
    colormap_blue[13] = 205 ;

    colormap_red[14] = 102 ;
    colormap_green[14] = 205 ;
    colormap_blue[14] = 0 ;

    colormap_red[15] = 240 ;
    colormap_green[15] = 255 ;
    colormap_blue[15] = 240 ;

    colormap_red[16] = 180 ;
    colormap_green[16] = 205 ;
    colormap_blue[16] = 205 ;

    colormap_red[17] = 255 ;
    colormap_green[17] = 245 ;
    colormap_blue[17] = 238 ;

    colormap_red[18] = 205 ;
    colormap_green[18] = 133 ;
    colormap_blue[18] = 63 ;

    colormap_red[19] = 108 ;
    colormap_green[19] = 133 ;
    colormap_blue[19] = 139 ;

    colormap_red[20] = 171 ;
    colormap_green[20] = 130 ;
    colormap_blue[20] = 255 ;

    colormap_red[21] = 255 ;
    colormap_green[21] = 228 ;
    colormap_blue[21] = 196 ;

    colormap_red[22] = 238 ;
    colormap_green[22] = 92 ;
    colormap_blue[22] = 56 ;

    colormap_red[23] = 0 ;
    colormap_green[23] = 205  ;
    colormap_blue[23] = 205 ;

    colormap_red[24] = 178 ;
    colormap_green[24] = 34 ;
    colormap_blue[24] = 34 ;

    colormap_red[25] = 139 ;
    colormap_green[25] = 95 ;
    colormap_blue[25] = 101 ;

    colormap_red[26] = 255 ;
    colormap_green[26] = 228 ;
    colormap_blue[26] = 225 ;

    colormap_red[27] = 238 ;
    colormap_green[27] = 232 ;
    colormap_blue[27] = 170 ;

    colormap_red[28] = 239 ;
    colormap_green[28] = 0 ;
    colormap_blue[28] = 239 ;

    colormap_red[29] = 179 ;
    colormap_green[29] = 238 ;
    colormap_blue[29] = 58 ;

    colormap_red[30] = 121 ;
    colormap_green[30] = 160 ;
    colormap_blue[30] = 221 ;

    colormap_red[31] = 255 ;
    colormap_green[31] = 0 ;
    colormap_blue[31] = 0 ;

    colormap_red[32] = 205 ;
    colormap_green[32] = 198 ;
    colormap_blue[32] = 115 ;

    colormap_red[33] = 255 ;
    colormap_green[33] = 215 ;
    colormap_blue[33] = 185 ;

    colormap_red[34] = 238 ;
    colormap_green[34] = 54 ;
    colormap_blue[34] = 0 ;

    colormap_red[35] = 155 ;
    colormap_green[35] = 205 ;
    colormap_blue[35] = 155 ;

    colormap_red[36] = 173 ;
    colormap_green[36] = 255 ;
    colormap_blue[36] = 47 ;

    colormap_red[37] = 238 ;
    colormap_green[37] = 122 ;
    colormap_blue[37] = 233 ;

    colormap_red[38] = 0 ;
    colormap_green[38] = 0 ;
    colormap_blue[38] = 128 ;

    colormap_red[39] = 222 ;
    colormap_green[39] = 184 ;
    colormap_blue[39] = 135 ;

    colormap_red[40] = 205 ;
    colormap_green[40] = 41 ;
    colormap_blue[40] = 144 ;

    colormap_red[41] = 245 ;
    colormap_green[41] = 255 ;
    colormap_blue[41] = 250 ;

    colormap_red[42] = 138 ;
    colormap_green[42] = 43 ;
    colormap_blue[42] = 226 ;

    colormap_red[43] = 125 ;
    colormap_green[43] = 38 ;
    colormap_blue[43] = 205 ;

    colormap_red[44] = 0 ;
    colormap_green[44] = 139 ;
    colormap_blue[44] = 69 ;

    colormap_red[45] = 34 ;
    colormap_green[45] = 139 ;
    colormap_blue[45] = 34 ;

    colormap_red[46] = 238 ;
    colormap_green[46] = 58 ;
    colormap_blue[46] = 140 ;

    colormap_red[47] = 47 ;
    colormap_green[47] = 79 ;
    colormap_blue[47] = 79 ;

    colormap_red[48] = 188 ;
    colormap_green[48] = 143 ;
    colormap_blue[48] = 143 ;

    colormap_red[49] = 124 ;
    colormap_green[49] = 252 ;
    colormap_blue[49] = 0 ;

    colormap_red[50] = 255 ;
    colormap_green[50] = 228 ;
    colormap_blue[50] = 181 ;

    colormap_red[51] = 239 ;
    colormap_green[51] = 237 ;
    colormap_blue[51] = 161 ;

    colormap_red[52] = 65 ;
    colormap_green[52] = 105 ;
    colormap_blue[52] = 225 ;

    colormap_red[53] = 255;
    colormap_green[53] = 238 ;
    colormap_blue[53] = 104 ;

    colormap_red[54] = 238 ;
    colormap_green[54] = 106 ;
    colormap_blue[54] = 80 ;

    colormap_red[55] = 0 ;
    colormap_green[55] = 100 ;
    colormap_blue[55] = 0 ;

    colormap_red[56] = 204 ;
    colormap_green[56] = 190 ;
    colormap_blue[56] = 112 ;

    colormap_red[57] = 205 ;
    colormap_green[57] = 200 ;
    colormap_blue[57] = 177 ;

    colormap_red[58] = 0 ;
    colormap_green[58] = 255 ;
    colormap_blue[58] = 127 ;

    colormap_red[59] = 240 ;
    colormap_green[59] = 255 ;
    colormap_blue[59] = 255 ;

    colormap_red[60] = 160 ;
    colormap_green[60] = 82 ;
    colormap_blue[60] = 45 ;

    colormap_red[61] = 46 ;
    colormap_green[61] = 139 ;
    colormap_blue[61] = 87 ;

    colormap_red[62] = 0 ;
    colormap_green[62] = 197 ;
    colormap_blue[62] = 205 ;

    colormap_red[63] = 205 ;
    colormap_green[63] = 102 ;
    colormap_blue[63] = 0 ;

    colormap_red[64] = 205 ;
    colormap_green[64] = 96 ;
    colormap_blue[64] = 144 ;

    colormap_red[65] = 122 ;
    colormap_green[65] = 197 ;
    colormap_blue[65] = 205 ;

    colormap_red[66] = 176 ;
    colormap_green[66] = 48 ;
    colormap_blue[66] = 96 ;

    colormap_red[67] = 209 ;
    colormap_green[67] = 95 ;
    colormap_blue[67] = 238 ;

    colormap_red[68] = 255 ;
    colormap_green[68] = 240 ;
    colormap_blue[68] = 245 ;

    colormap_red[69] = 205 ;
    colormap_green[69] = 102 ;
    colormap_blue[69] = 29 ;

    colormap_red[70] = 79 ;
    colormap_green[70] = 148 ;
    colormap_blue[70] = 205 ;

    colormap_red[71] = 255 ;
    colormap_green[71] = 255 ;
    colormap_blue[71] = 240 ;

    colormap_red[72] = 208 ;
    colormap_green[72] = 32 ;
    colormap_blue[72] = 144 ;

    colormap_red[73] = 0 ;
    colormap_green[73] = 206 ;
    colormap_blue[73] = 209 ;

    colormap_red[74] = 141 ;
    colormap_green[74] = 238 ;
    colormap_blue[74] = 238 ;

    colormap_red[75] = 205 ;
    colormap_green[75] = 201 ;
    colormap_blue[75] = 201;

    colormap_red[76] = 143 ;
    colormap_green[76] = 188;
    colormap_blue[76] = 143 ;

    colormap_red[77] = 240 ;
    colormap_green[77] = 248 ;
    colormap_blue[77] = 255 ;

    colormap_red[78] = 255 ;
    colormap_green[78] = 255 ;
    colormap_blue[78] = 0 ;

    colormap_red[79] = 135 ;
    colormap_green[79] = 206 ;
    colormap_blue[79] = 235 ;

    colormap_red[80] = 220 ;
    colormap_green[80] = 220 ;
    colormap_blue[80] = 220 ;

    colormap_red[81] = 238 ;
    colormap_green[81] = 99 ;
    colormap_blue[81] = 99 ;

    colormap_red[82] = 100 ;
    colormap_green[82] = 149 ;
    colormap_blue[82] = 237 ;

    colormap_red[83] = 255 ;
    colormap_green[83] = 235 ;
    colormap_blue[83] = 205 ;

    colormap_red[84] = 238 ;
    colormap_green[84] = 153 ;
    colormap_blue[84] = 73 ;

    colormap_red[85] = 238 ;
    colormap_green[85] = 18 ;
    colormap_blue[85] = 137 ;

    colormap_red[86] = 25 ;
    colormap_green[86] = 25 ;
    colormap_blue[86] = 112 ;

    colormap_red[87] = 205 ;
    colormap_green[87] = 112 ;
    colormap_blue[87] = 84 ;

    colormap_red[88] = 60 ;
    colormap_green[88] = 179 ;
    colormap_blue[88] = 113 ;

    colormap_red[89] = 255 ;
    colormap_green[89] = 250 ;
    colormap_blue[89] = 205 ;

    colormap_red[90] = 139 ;
    colormap_green[90] = 26 ;
    colormap_blue[90] = 26 ;

    colormap_red[91] = 173 ;
    colormap_green[91] = 216 ;
    colormap_blue[91] = 230 ;

    colormap_red[92] = 211 ;
    colormap_green[92] = 211 ;
    colormap_blue[92] = 211 ;

    colormap_red[93] = 205 ;
    colormap_green[93] = 192 ;
    colormap_blue[93] = 176 ;

    colormap_red[94] = 238 ;
    colormap_green[94] = 213 ;
    colormap_blue[94] = 210 ;

    colormap_red[95] = 110 ;
    colormap_green[95] = 139 ;
    colormap_blue[95] = 61 ;

    colormap_red[96] = 139 ;
    colormap_green[96] = 134;
    colormap_blue[96] = 130 ;

    colormap_red[97] = 102 ;
    colormap_green[97] = 205 ;
    colormap_blue[97] = 170 ;

    colormap_red[98] = 205 ;
    colormap_green[98] = 205 ;
    colormap_blue[98] = 180 ;

    colormap_red[99]   = 205 ;
    colormap_green[99] = 145 ;
    colormap_blue[99]  = 115 ;

    colormap_red[100]   = 0 ;
    colormap_green[100] = 229 ;
    colormap_blue[100]  = 238 ;

    colormap_red[101]   = 205 ;
    colormap_green[101] = 104 ;
    colormap_blue[101]  = 57 ;

    colormap_red[102]   = 32 ;
    colormap_green[102] = 178 ;
    colormap_blue[102]  = 170 ;

    colormap_red[103]   = 137 ;
    colormap_green[103] = 104 ;
    colormap_blue[103]  = 205 ;

    colormap_red[104]   = 139 ;
    colormap_green[104] = 69 ;
    colormap_blue[104]  = 19 ;

    colormap_red[105]   = 132 ;
    colormap_green[105] = 112 ;
    colormap_blue[105]  = 255 ;

    colormap_red[106]   = 255 ;
    colormap_green[106] = 246 ;
    colormap_blue[106]  = 143 ;

    colormap_red[107]   = 216 ;
    colormap_green[107] = 191 ;
    colormap_blue[107]  = 216 ;

    colormap_red[108]   = 154 ;
    colormap_green[108] = 50 ;
    colormap_blue[108]  = 205 ;

    colormap_red[109]   = 253 ;
    colormap_green[109] = 245 ;
    colormap_blue[109]  = 230 ;

    colormap_red[110]   = 239 ;
    colormap_green[110] = 87 ;
    colormap_blue[110]  = 66 ;

    colormap_red[111]   = 205 ;
    colormap_green[111] = 104 ;
    colormap_blue[111]  = 137 ;

    colormap_red[112]   = 102 ;
    colormap_green[112] = 205 ;
    colormap_blue[112]  = 170 ;

    colormap_red[113]   = 205 ;
    colormap_green[113] = 133 ;
    colormap_blue[113]  = 0 ;

    colormap_red[114]   = 239 ;
    colormap_green[114] = 62 ;
    colormap_blue[114]  = 47 ;

    colormap_red[115]   = 105 ;
    colormap_green[115] = 105 ;
    colormap_blue[115]  = 105 ;

    colormap_red[116]   = 139 ;
    colormap_green[116] = 125 ;
    colormap_blue[116]  = 107 ;

    colormap_red[117]   = 0 ;
    colormap_green[117] = 0 ;
    colormap_blue[117]  = 205 ;

    colormap_red[118]   = 105 ;
    colormap_green[118] = 139 ;
    colormap_blue[118]  = 34 ;

    colormap_red[119]   = 184 ;
    colormap_green[119] = 134 ;
    colormap_blue[119]  = 11 ;

    colormap_red[120]   = 70 ;
    colormap_green[120] = 130 ;
    colormap_blue[120]  = 180 ;

    colormap_red[121]   = 84 ;
    colormap_green[121] = 139 ;
    colormap_blue[121]  = 84 ;

    colormap_red[122]   = 255 ;
    colormap_green[122] = 48 ;
    colormap_blue[122]  = 48 ;

    colormap_red[123]   = 238 ;
    colormap_green[123] = 174 ;
    colormap_blue[123]  = 238 ;

    colormap_red[124]   = 112 ;
    colormap_green[124] = 128 ;
    colormap_blue[124]  = 144 ;

    colormap_red[125]   = 255 ;
    colormap_green[125] = 69 ;
    colormap_blue[125]  = 0 ;

    colormap_red[126]   = 154 ;
    colormap_green[126] = 192 ;
    colormap_blue[126]  = 205 ;

    for (i = 127, j = 2 ; j < 127 ;i++, j++) {
	colormap_red[i] = colormap_red[j] ;
	colormap_green[i] = colormap_green[j] ;
	colormap_blue[i] = colormap_blue[j] ;
      }

    for(i=0 ;i < CMAPSIZE-1; i++){
      clist[i].red = colormap_red[i] << 8;
      clist[i].green = colormap_green[i] << 8;
      clist[i].blue = colormap_blue[i] << 8;
      clist[i].pixel = colors[i];
      clist[i].flags = DoRed | DoGreen | DoBlue;
    }
    clist[CMAPSIZE-1].red = 255 << 8;
    clist[CMAPSIZE-1].green = 255 << 8;
    clist[CMAPSIZE-1].blue = 255 << 8;
    clist[CMAPSIZE-1].pixel = colors[CMAPSIZE-1];
    clist[CMAPSIZE-1].flags = DoRed | DoGreen | DoBlue;
}
