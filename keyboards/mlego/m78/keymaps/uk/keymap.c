/*
Copyright 2021-2022 Alin M Elena <alinm.elena@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include QMK_KEYBOARD_H
#include "keymap_uk.h"
#ifdef CONSOLE_ENABLE
#include "print.h"
#endif

const uint32_t PROGMEM unicode_map[] = {
    [la]  = 0x03B1 , // α
    [lA]  = 0x0307 , //
    [lb]  = 0x03B2 , // β
    [lB]  = 0x2229 , // ∩
    [lc]  = 0x03C7 , // χ
    [lC]  = 0x2282 , // ⊂
    [ld]  = 0x03B4 , // δ
    [lD]  = 0x2202 , // ∂
    [le]  = 0x03B5 , // ε
    [lE]  = 0x2107 , // ℇ
    [lf]  = 0x03C6 , // φ
    [lF]  = 0x03C8 , // ψ
    [lg]  = 0x03B3 , // γ
    [lG]  = 0x0393 , // Γ
    [lh]  = 0x210F , // ℏ
    [lH]  = 0x1D4D7, // 𝓗
    [li]  = 0x222B , // ∫
    [lI]  = 0x222E , // ∮
    [lj]  = 0x2208 , // ∈
    [lJ]  = 0x2209 , // ∉
    [lk]  = 0x03F0 , // ϰ
    [lK]  = 0x2206 , // ∆
    [ll]  = 0x03BB , // λ
    [lL]  = 0x039B , // Λ
    [lm]  = 0x03BC , // μ
    [lM]  = 0x2218 , // ∘
    [ln]  = 0x03B7 , // η
    [lN]  = 0x222A , // ∪
    [lo]  = 0x221E , // ∞
    [lO]  = 0x2297 , // ⊗
    [lp]  = 0x03C0 , // π
    [lP]  = 0x220F , // ∏
    [lq]  = 0x03C3 , // σ
    [lQ]  = 0x03D5 , // ϕ
    [lr]  = 0x03C1 , // ρ
    [lR]  = 0x2207 , // ∇
    [ls]  = 0x2211 , // ∑
    [lS]  = 0x2A0B , // ⨋
    [lt]  = 0x03D1 , // ϑ
    [lT]  = 0x03B8 , // θ
    [lu]  = 0x03C4 , // τ
    [lU]  = 0x2102 , // ℂ
    [lv]  = 0x03BD , // χ
    [lV]  = 0x2283 , // ⊃
    [lw]  = 0x03C9 , // ω
    [lW]  = 0x03A9 , // Ω
    [lx]  = 0x03BE , // ξ
    [lX]  = 0x039E , // Ξ
    [ly]  = 0x211d , // ℝ
    [lY]  = 0x2124 , // ℤ
    [lz]  = 0x03B6 , // ζ
    [lZ]  = 0x2221 , // ∡
    [lc1] = 0x2248 , // ≈
    [lC1] = 0x2249 , // ≉
    [lp1] = 0x00B1 , // ±
    [lP1] = 0x2213 , // ∓
    [lq1] = 0x00D7 , // ×
    [lQ1] = 0x22C5 , // ⋅
    [ll1] = 0x1D53C, // 𝔼
    [lL1] = 0x212b , // Å
    [lk1] = 0x221D , // ∝
    [lK1] = 0x2112 , // ℒ
    [lbo1] = 0x22DC , //⋜
    [lbo2] = 0x2260 , //≠
    [lbc1] = 0x22DD , // ⋝
    [lbc2] = 0x2245 , //≅
    [ls1] = 0x2261 , //≡
    [ls2] = 0x2262 , // ≢
    [ra]  = 0x0103 , // ă
    [rA]  = 0x0102 , // Ă
    [ra1] = 0x00E2 , // â
    [rA1] = 0x00C2 , // Â
    [rx]  = 0x00E7 , // ç
    [rX]  = 0x00C7 , // Ç
    [ri]  = 0x00EE , // î
    [rI]  = 0x00CE , // Î
    [rs]  = 0x0219 , // ș⊄
    [rS]  = 0x0218 , // Ș
    [rt]  = 0x021B , // ț
    [rT]  = 0x021A , // Ț
    [rc] = 0x2284, // ⊄
    [rC] = 0x2286, // ⊆
    [rv] = 0x2285, // ⊅
    [rV] = 0x2287, //⊇
    [rb] = 0x2227, // ∧
    [rB] = 0x22BC, //⊼
    [rn] = 0x2228, //∨
    [rN] = 0x22BB, //⊻
};

#if defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_map[][NUM_ENCODERS][2] = {
    [_QW]  = { ENCODER_CCW_CW(KC_VOLD, KC_VOLU) },
    [_LWR] = { ENCODER_CCW_CW(RGB_HUD, RGB_HUI) },
    [_RSE] = { ENCODER_CCW_CW(RGB_VAD, RGB_VAI) },
    [_ADJ] = { ENCODER_CCW_CW(RGB_RMOD, RGB_MOD) },
};
#endif

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ ⎋ │ 1 │ 2 │ 3 │ 4 │ 5 │ 6 │ 7 │ 8 │ 9 │ 0 │ - │ ⌫ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ↹ │ q │ w │ e │ r │ t │ y │ u │ i │ o │ p │ [ │ ] │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ # │ a │ s │ d │ f │ g │ h │ j │ k │ l │ ; │ ' │ ⏎ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ⇧(│ \ │ z │ x │ c │ v │ b │ n │ m │ , │ , │ ↑ │ / │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ⎈ │mnu│lwr│⎇  │rse│   │   │   │⎇  │ ⇧)│ ← │ ↓ │ → │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
shifted symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ ← │ ! │ " │ £ │ $ │ % │ ^ │ & │ * │ ( │ ) │ _ │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ Q │ W │ E │ R │ T │ Y │ U │ I │ O │ P │ { │ } │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ~ │ A │ S │ D │ F │ G │ H │ J │ K │ L │ : │ @ │ ⏎ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ⇧(│ | │ Z │ X │ C │ V │ B │ N │ M │ < │ > │   │ / │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │   │   │ ⇧)│   │   │   │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
altgr symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ ← │ ¹ │ ² │ ³ │ € │ ½ │ ¾ │ { │ [ │ ] │ } │ \ │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ @ │ ſ │ e │ ¶ │ ŧ │ ← │ ↓ │ → │ ø │ þ │ ä │ ã │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ~ │ æ │ ß │ ð │ đ │ ŋ │ ħ │ ả │ ĸ │ ł │ á │ â │ent│
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ± │ | │ « │ » │ ¢ │ „ │ “ │ ” │ µ │ ─ │ · │   │ ạ̣̣ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │   │   │ ° │   │   │   │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
altgr shift symbols
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │   │ ¡ │ ⅛ │ £ │ ¼ │ ⅜ │ ⅝ │ ⅞ │ ™ │ ± │ ° │ ¿ │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ Ω │ § │ E │ ® │ Ŧ │ ¥ │ ↑ │ ı │ Ø │ Þ │ å │ ā │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ Æ │ ẞ │ Ð │ ª │ Ŋ │ Ħ │ a │ & │ Ł │ ˝a│ ǎ │ent│
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ± │ ¦ │ ‹ │ › │ © │ ‚ │ ‘ │ ’ │ º │ × │ ÷ │   │ ȧ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │   │   │ ° │   │   │   │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
 *
*/
  [_QW] = LAYOUT_ortho_6x13(
      KC_ESC ,    KC_F1  ,   KC_F2  , KC_F3  ,  KC_F4  ,  KC_F5  , KC_F6  , KC_F7  ,  KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12  ,
       KC_GRV, UK_1   , UK_2    , UK_3   , UK_4    , UK_5  , UK_6  , UK_7  , UK_8   , UK_9   , UK_0   , UK_MINS, KC_BSPC ,
       KC_TAB, UK_Q   , UK_W    , UK_E   , UK_R    , UK_T  , UK_Y  , UK_U  , UK_I   , UK_O   , UK_P   , UK_LBRC, UK_RBRC ,
      UK_HASH, UK_A   , UK_S    , UK_D   , UK_F    , UK_G  , UK_H  , UK_J  , UK_K   , UK_L   , UK_SCLN, UK_QUOT, KC_ENT  ,
      SC_LSPO, UK_BSLS, UK_Z    , UK_X   , UK_C    , UK_V  , UK_B  , UK_N  , UK_M   , UK_COMM, UK_DOT , KC_UP  , UK_SLSH ,
      KC_LCTL, KC_LGUI, TT(_LWR), KC_LALT, TT(_RSE), KC_SPC, KC_SPC, KC_SPC, KC_RALT, SC_RSPC, KC_LEFT, KC_DOWN, KC_RGHT),

/*
raise layer
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ ` │MUT│Vup│Vdn│prv│ply│nxt│EXT│SLP│WAK│PSC│ ⌫ │ = │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ σ │ ω │ ε │ ρ │ ϑ │ ℝ │ τ │ ∫ │ ∞ │ π │ ⋜ │ ⋝ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ α │ ∑ │ δ │ φ │ γ │ ℏ │ ∈ │ ϰ │ λ │ 𝔼 │ ∝ │ ⏎ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ⇧(│ ≡ │ ζ │ ξ │ χ │ ν │ β │ η │ μ │ ≈ │ ± │   │ × │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │lwr│⎇  │rse│   │   │   │⎇  │ ⇧)│   │   │   │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
raise layer shifted
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ ¬ │   │   │   │   │   │   │   │   │   │   │ ⌫ │ + │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │MB3│ ϕ │ Ω │ ℇ │ ∇ │ θ │ ℤ │ ℂ │ ∮ │ ⊗ │ ∏ │ ≠ │ ≅ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │MB2│  ̇ │ ⨋ │ ∂ │ ψ │ Γ │ 𝓗 │ ∉ │ ∆ │ Λ │ Å │ ℒ │ ⏎ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ⇧(│ ≢ │ ∡ │ Ξ │ ⊂ │ ⊃ │ ∩ │ ∪ │ ∘ │ ≉ │ ∓ │MSU│ ⋅ │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │MB1│MB4│lwr│⎇  │rse│   │   │   │⎇  │ ⇧)│MSL│MSD│MSR│
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
MB - mouse buttons
MS - mouse directions
*/
 [_RSE] = LAYOUT_ortho_6x13(
      _______, _______, _______, _______, _______, _______ , _______, _______, _______,_______,_______,_______,_______,
     KC_GRV ,    KC_MUTE,  KC_VOLU,  KC_VOLD,  KC_MPRV,  KC_MPLY,  KC_MNXT,  G(KC_P),  KC_SLEP,    KC_WAKE,    KC_PSCR,        KC_DEL,        UK_EQL,
     KC_BTN3,  XP(lq,lQ),XP(lw,lW),XP(le,lE),XP(lr,lR),XP(lt,lT),XP(ly,lY),XP(lu,lU),XP(li,lI),  XP(lo,lO),  XP(lp,lP), XP(lbo1,lbo2), XP(lbc1,lbc2),
     KC_BTN2,  XP(la,lA),XP(ls,lS),XP(ld,lD),XP(lf,lF),XP(lg,lG),XP(lh,lH),XP(lj,lJ),XP(lk,lK),  XP(ll,lL),XP(ll1,lL1),   XP(lk1,lK1),       _______,
     _______,XP(ls1,ls2),XP(lz,lZ),XP(lx,lX),XP(lc,lC),XP(lv,lV),XP(lb,lB),XP(ln,lN),XP(lm,lM),XP(lc1,lC1),XP(lp1,lP1),       KC_MS_U,   XP(lq1,lQ1),
     KC_BTN1,    KC_BTN4,  _______,  _______,  _______,  _______,  _______,  _______,  _______,    _______,    KC_MS_L,       KC_MS_D,      KC_MS_R),

/*
lower layer
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │ ⎋ │ F1│ F2│ F3│ F4│ F5│ F6│ F7│ F8│ F9│F10│F11│F12│
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ â │   │   │   │ ț │   │   │ î │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │ ⇪ │ ă │ ș │   │   │   │   │   │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │ ç │ ⊄ │ ⊅ │ ∧ │ ∨ │   │   │   │MWU│   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │   │   │   │MWL│MWD│MWR│
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
MW - mouse wheel
raise layer shifted
 * ┌───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┬───┐
 * │   │   │   │   │   │   │   │   │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ Â │   │   │   │ Ț │   │   │ Î │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │ Ă │ Ș │   │   │   │   │   │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │ Ç │ ⊆ │ ⊇ │ ⊼ │ ⊻ │   │   │   │   │   │
 * ├───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┼───┤
 * │   │   │   │   │   │   │   │   │   │   │   │   │   │
 * └───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┴───┘
*/
  [_LWR] = LAYOUT_ortho_6x13(
      _______, _______, _______, _______, _______, _______ , _______, _______, _______,_______,_______,_______,_______,
      KC_ESC ,    KC_F1  ,   KC_F2  , KC_F3  ,  KC_F4  ,  KC_F5  , KC_F6  , KC_F7  ,  KC_F8  , KC_F9  , KC_F10 , KC_F11 , KC_F12  ,
      _______,XP(ra1,rA1),   _______, _______,  _______,XP(rt,rT), _______, _______,XP(ri,rI), _______, _______, _______, _______ ,
      KC_CAPS,  XP(ra,rA), XP(rs,rS), _______,  _______,  _______, _______, _______,  _______, _______, _______, _______, _______ ,
      _______,    KC_F20 ,   _______, XP(rx,rX),XP(rc,rC),XP(rv,rV) , XP(rb,rB), XP(rn,rN),  _______, _______, _______, KC_WH_U, _______ ,
      _______,    _______,   _______, _______,  _______,  _______, _______, _______,  _______, _______, KC_WH_L, KC_WH_D, KC_WH_R),
/*
adj layer
 * ┌───┬────┬───┬───┬───┬───┬───┬───┬───┬───┬───┬────┬────┐
 * │MOD│RMOD│   │   │   │   │   │   │   │   │   │M_T │M_SW│
 * ├───┼────┼───┼───┼───┼───┼───┼───┼───┼───┼───┼────┼────┤
 * │HUI│HUD │M_P│   │rst│   │   │   │   │   │   │    │M_SN│
 * ├───┼────┼───┼───┼───┼───┼───┼───┼───┼───┼───┼────┼────┤
 * │SAI│SAD │M_B│   │   │   │   │   │   │   │   │    │M_K │
 * ├───┼────┼───┼───┼───┼───┼───┼───┼───┼───┼───┼────┼────┤
 * │VAI│VAD │M_R│   │   │   │   │   │   │   │   │    │M_X │
 * ├───┼────┼───┼───┼───┼───┼───┼───┼───┼───┼───┼────┼────┤
 * │TOG│    │   │   │   │   │   │   │   │   │   │M_TW│M_G │
 * └───┴────┴───┴───┴───┴───┴───┴───┴───┴───┴───┴────┴────┘
*/
  [_ADJ] = LAYOUT_ortho_6x13(
      _______, _______, _______, _______, _______, _______ , _______, _______, _______,_______,_______,_______,_______,
      RGB_MOD, RGB_RMOD, A(KC_F2), _______, _______, _______, _______, _______, _______, _______, _______, RGB_M_T , RGB_M_SW,
      RGB_HUI, RGB_HUD , RGB_M_P , _______, QK_BOOT, _______, _______, _______, _______, _______, _______, _______ , RGB_M_SN,
      RGB_SAI, RGB_SAD , RGB_M_B , _______, UC_RMOD, _______, _______, _______, _______, _______, _______, _______ , RGB_M_K ,
      RGB_VAI, RGB_VAD , RGB_M_R , _______,  UC_MOD, UC_M_LN, UC_M_WI, UC_M_WC, UC_M_MA, _______, _______, _______ , RGB_M_X ,
      RGB_TOG, _______ , _______ , _______, _______, _______, _______, _______, _______, _______, _______, RGB_M_TW, RGB_M_G),
};
// clang-format on

bool led_update_user(led_t led_state) {
    // Disable the default LED update code, so that lock LEDs could be reused to show layer status.
    return false;
}

void matrix_scan_user(void) {

    toggle_leds();

}

bool process_record_user(uint16_t keycode, keyrecord_t* record) {
#ifdef CONSOLE_ENABLE
    uprintf("KL: kc: 0x%04X, col: %2u, row: %2u, pressed: %u, time: %5u, int: %u, count: %u\n", keycode, record->event.key.col, record->event.key.row, record->event.pressed, record->event.time, record->tap.interrupted, record->tap.count);
#endif

    switch (keycode) {
        case (TT(_LWR)):
            if (!record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                // This runs before the TT() handler toggles the layer state, so the current layer state is the opposite of the final one after toggle.
                set_led_toggle(_LWR, !layer_state_is(_LWR));
            }
            return true;
            break;
        case (TT(_RSE)):
            if (record->event.pressed && record->tap.count == TAPPING_TOGGLE) {
                set_led_toggle(_RSE, !layer_state_is(_RSE));
            }
            return true;
            break;
        default:
            return true;
    }
}

layer_state_t layer_state_set_user(layer_state_t state) {

    layer_state_t st = update_tri_layer_state(state, _LWR, _RSE, _ADJ);

#ifdef RGBLIGHT_ENABLE

   set_rgb_layers(state);

#endif

    return st;
}

#ifdef RGBLIGHT_ENABLE

layer_state_t default_layer_state_set_user(layer_state_t state) {

    set_default_rgb_layers(state);
    return state;
}

#endif

void keyboard_post_init_user(void) {

#ifdef RGBLIGHT_ENABLE
#ifdef RGB_ENABLE_PIN
    setPinOutput(RGB_ENABLE_PIN);
    writePinHigh(RGB_ENABLE_PIN);
    wait_ms(30);
#endif
  // Enable the LED layers
    rgblight_layers = my_rgb();

#endif

#ifdef OLED_ENABLE
    init_timer();
#endif
#ifdef CONSOLE_ENABLE
  debug_enable = true;
  debug_matrix = true;
  debug_keyboard = true;
#endif
}
