// lmch.h -- Lisp Machine character set

#ifndef USIM_LMCH_H
#define USIM_LMCH_H

// See SYS:LMIO;RDDEFS LISP for details.

#define LMCH_null 0200
#define LMCH_null_character 0200
#define LMCH_break 0201
#define LMCH_brk 0201
#define LMCH_suspend 0201
#define LMCH_clear_input 0202
#define LMCH_clear 0202
#define LMCH_clr 0202
#define LMCH_call 0203
#define LMCH_terminal 0204
#define LMCH_esc 0204
#define LMCH_escape 0204
#define LMCH_terminal_escape 0204
#define LMCH_function 0204
#define LMCH_macro 0205
#define LMCH_back_next 0205
#define LMCH_backnext 0205
#define LMCH_help 0206
#define LMCH_rubout 0207
#define LMCH_overstrike 0210
#define LMCH_backspace 0210
#define LMCH_bs 0210
#define LMCH_tab 0211
#define LMCH_line 0212
#define LMCH_lf 0212
#define LMCH_linefeed 0212
#define LMCH_line_feed 0212
#define LMCH_delete 0213
#define LMCH_vt 0213
// The keyboard says "CLEAR SCREEN", but it should type out as "PAGE".
#define LMCH_page 0214
#define LMCH_clear_screen 0214
#define LMCH_form 0214
#define LMCH_ff 0214
#define LMCH_return 0215
#define LMCH_newline 0215
#define LMCH_cr 0215
#define LMCH_quote 0216
#define LMCH_hold_output 0217
#define LMCH_stop_output 0220
#define LMCH_abort 0221
#define LMCH_resume 0222
#define LMCH_status 0223
#define LMCH_end 0224
#define LMCH_roman_i 0225
#define LMCH_roman_ii 0226
#define LMCH_roman_iii 0227
#define LMCH_roman_iv 0230
#define LMCH_hand_up 0231
#define LMCH_hand_down 0232
#define LMCH_hand_left 0233
#define LMCH_hand_right 0234
#define LMCH_system 0235
#define LMCH_select 0235
#define LMCH_network 0236
#define LMCH_center_dot 00
#define LMCH_centre_dot 00	// Amerikans can't spell...
#define LMCH_down_arrow 01
#define LMCH_alpha 02
#define LMCH_beta 03
#define LMCH_and_sign 04
#define LMCH_not_sign 05
#define LMCH_epsilon 06
#define LMCH_pi 07
#define LMCH_lambda 010
#define LMCH_gamma 011
#define LMCH_delta 012
#define LMCH_up_arrow 013
#define LMCH_uparrow 013
#define LMCH_plus_minus 014
#define LMCH_circle_plus 015
#define LMCH_infinity 016
#define LMCH_partial_delta 017
#define LMCH_left_horseshoe 020
#define LMCH_right_horseshoe 021
#define LMCH_up_horseshoe 022
#define LMCH_down_horseshoe 023
#define LMCH_universal_quantifier 024
#define LMCH_for_all 024
#define LMCH_existential_quantifier 025
#define LMCH_there_exists 025
#define LMCH_circle_x 026
#define LMCH_circle_cross 026
#define LMCH_tensor 026
#define LMCH_double_arrow 027
#define LMCH_left_arrow 030
#define LMCH_right_arrow 031
#define LMCH_not_equal 032
#define LMCH_not_equals 032
#define LMCH_altmode 033
#define LMCH_alt 033
#define LMCH_diamond 033
#define LMCH_less_or_equal 034
#define LMCH_greater_or_equal 035
#define LMCH_equivalence 036
#define LMCH_or_sign 037
#define LMCH_or 037
#define LMCH_space 040
#define LMCH_sp 040
#define LMCH_integral 0177
// sigh. 259 > char-code-limit
#define LMCH_coke_bottle  (128 + 69)
#define LMCH_cokebottle (128 + 69)

// X11 compatible names for keysyms.
#define LMCH_exclam 041
#define LMCH_quotedbl 042
#define LMCH_numbersign 043
#define LMCH_dollar 044
#define LMCH_percent 045
#define LMCH_ampersand 046
#define LMCH_apostrophe 047
#define LMCH_parenleft 050
#define LMCH_parenright 051
#define LMCH_asterisk 052
#define LMCH_plus 053
#define LMCH_comma 054
#define LMCH_minus 055
#define LMCH_period 056
#define LMCH_slash 057
#define LMCH_0 060
#define LMCH_1 061
#define LMCH_2 062
#define LMCH_3 063
#define LMCH_4 064
#define LMCH_5 065
#define LMCH_6 066
#define LMCH_7 067
#define LMCH_8 070
#define LMCH_9 071
#define LMCH_colon 072
#define LMCH_semicolon 073
#define LMCH_less 074
#define LMCH_equal 075
#define LMCH_greater 076
#define LMCH_question 077
#define LMCH_at 0100
#define LMCH_A 0101
#define LMCH_B 0102
#define LMCH_C 0103
#define LMCH_D 0104
#define LMCH_E 0105
#define LMCH_F 0106
#define LMCH_G 0107
#define LMCH_H 0110
#define LMCH_I 0111
#define LMCH_J 0112
#define LMCH_K 0113
#define LMCH_L 0114
#define LMCH_M 0115
#define LMCH_N 0116
#define LMCH_O 0117
#define LMCH_P 0120
#define LMCH_Q 0121
#define LMCH_R 0122
#define LMCH_S 0123
#define LMCH_T 0124
#define LMCH_U 0125
#define LMCH_V 0126
#define LMCH_W 0127
#define LMCH_X 0130
#define LMCH_Y 0131
#define LMCH_Z 0132
#define LMCH_bracketleft 0133
#define LMCH_backslash 0134
#define LMCH_bracketright 0135
#define LMCH_asciicircum 0136
#define LMCH_underscore 0137
#define LMCH_grave 0140
#define LMCH_a 0141
#define LMCH_b 0142
#define LMCH_c 0143
#define LMCH_d 0144
#define LMCH_e 0145
#define LMCH_f 0146
#define LMCH_g 0147
#define LMCH_h 0150
#define LMCH_i 0151
#define LMCH_j 0152
#define LMCH_k 0153
#define LMCH_l 0154
#define LMCH_m 0155
#define LMCH_n 0156
#define LMCH_o 0157
#define LMCH_p 0160
#define LMCH_q 0161
#define LMCH_r 0162
#define LMCH_s 0163
#define LMCH_t 0164
#define LMCH_u 0165
#define LMCH_v 0166
#define LMCH_w 0167
#define LMCH_x 0170
#define LMCH_y 0171
#define LMCH_z 0172
#define LMCH_braceleft 0173
#define LMCH_bar 0174
#define LMCH_braceright 0175
#define LMCH_asciitilde 0176

#endif
