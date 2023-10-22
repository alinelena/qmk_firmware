"""Functions that help us work with keyboards.
"""
from array import array
from functools import lru_cache
from math import ceil
from pathlib import Path
import os
from glob import glob
from qmk.constants import COL_LETTERS, ROW_LETTERS

import qmk.path
from qmk.c_parse import parse_config_h_file
from qmk.json_schema import json_load
from qmk.makefile import parse_rules_mk_file

from collections import defaultdict
from draw_keyboard import draw_kb,rad,swx,swy,rox,roy,cox,coy,isoenter_key,baeenter_key
from layers import get_key_labels

BOX_DRAWING_CHARACTERS = {
    "unicode": {
        "tl": "┌",
        "tr": "┐",
        "bl": "└",
        "br": "┘",
        "v": "│",
        "h": "─",
    },
    "ascii": {
        "tl": " ",
        "tr": " ",
        "bl": "|",
        "br": "|",
        "v": "|",
        "h": "_",
    },
}
ENC_DRAWING_CHARACTERS = {
    "unicode": {
        "tl": "╭",
        "tr": "╮",
        "bl": "╰",
        "br": "╯",
        "vl": "▲",
        "vr": "▼",
        "v": "│",
        "h": "─",
    },
    "ascii": {
        "tl": " ",
        "tr": " ",
        "bl": "\\",
        "br": "/",
        "v": "|",
        "vl": "/",
        "vr": "\\",
        "h": "_",
    },
}


class AllKeyboards:
    """Represents all keyboards.
    """
    def __str__(self):
        return 'all'

    def __repr__(self):
        return 'all'

    def __eq__(self, other):
        return isinstance(other, AllKeyboards)


base_path = os.path.join(os.getcwd(), "keyboards") + os.path.sep


@lru_cache(maxsize=1)
def keyboard_alias_definitions():
    return json_load(Path('data/mappings/keyboard_aliases.hjson'))


def is_all_keyboards(keyboard):
    """Returns True if the keyboard is an AllKeyboards object.
    """
    if isinstance(keyboard, str):
        return (keyboard == 'all')
    return isinstance(keyboard, AllKeyboards)


def find_keyboard_from_dir():
    """Returns a keyboard name based on the user's current directory.
    """
    relative_cwd = qmk.path.under_qmk_userspace()
    if not relative_cwd:
        relative_cwd = qmk.path.under_qmk_firmware()

    if relative_cwd and len(relative_cwd.parts) > 1 and relative_cwd.parts[0] == 'keyboards':
        # Attempt to extract the keyboard name from the current directory
        current_path = Path('/'.join(relative_cwd.parts[1:]))

        if 'keymaps' in current_path.parts:
            # Strip current_path of anything after `keymaps`
            keymap_index = len(current_path.parts) - current_path.parts.index('keymaps') - 1
            current_path = current_path.parents[keymap_index]

        if qmk.path.is_keyboard(current_path):
            return str(current_path)


def find_readme(keyboard):
    """Returns the readme for this keyboard.
    """
    cur_dir = qmk.path.keyboard(keyboard)
    keyboards_dir = Path('keyboards')
    while not (cur_dir / 'readme.md').exists():
        if cur_dir == keyboards_dir:
            return None
        cur_dir = cur_dir.parent

    return cur_dir / 'readme.md'


def keyboard_folder(keyboard):
    """Returns the actual keyboard folder.

    This checks aliases and DEFAULT_FOLDER to resolve the actual path for a keyboard.
    """
    aliases = keyboard_alias_definitions()

    while keyboard in aliases:
        last_keyboard = keyboard
        keyboard = aliases[keyboard].get('target', keyboard)
        if keyboard == last_keyboard:
            break

    keyboard = resolve_keyboard(keyboard)

    if not qmk.path.is_keyboard(keyboard):
        raise ValueError(f'Invalid keyboard: {keyboard}')

    return keyboard


def keyboard_aliases(keyboard):
    """Returns the list of aliases for the supplied keyboard.

    Includes the keyboard itself.
    """
    aliases = json_load(Path('data/mappings/keyboard_aliases.hjson'))

    if keyboard in aliases:
        keyboard = aliases[keyboard].get('target', keyboard)

    keyboards = set(filter(lambda k: aliases[k].get('target', '') == keyboard, aliases.keys()))
    keyboards.add(keyboard)
    keyboards = list(sorted(keyboards))
    return keyboards


def keyboard_folder_or_all(keyboard):
    """Returns the actual keyboard folder.

    This checks aliases and DEFAULT_FOLDER to resolve the actual path for a keyboard.
    If the supplied argument is "all", it returns an AllKeyboards object.
    """
    if keyboard == 'all':
        return AllKeyboards()

    return keyboard_folder(keyboard)


def _find_name(path):
    """Determine the keyboard name by stripping off the base_path and rules.mk.
    """
    return path.replace(base_path, "").replace(os.path.sep + "rules.mk", "")


def keyboard_completer(prefix, action, parser, parsed_args):
    """Returns a list of keyboards for tab completion.
    """
    return list_keyboards()


def list_keyboards(resolve_defaults=True):
    """Returns a list of all keyboards - optionally processing any DEFAULT_FOLDER.
    """
    # We avoid pathlib here because this is performance critical code.
    kb_wildcard = os.path.join(base_path, "**", "rules.mk")
    paths = [path for path in glob(kb_wildcard, recursive=True) if os.path.sep + 'keymaps' + os.path.sep not in path]

    found = map(_find_name, paths)
    if resolve_defaults:
        found = map(resolve_keyboard, found)

    return sorted(set(found))


@lru_cache(maxsize=None)
def resolve_keyboard(keyboard):
    cur_dir = Path('keyboards')
    rules = parse_rules_mk_file(cur_dir / keyboard / 'rules.mk')
    while 'DEFAULT_FOLDER' in rules and keyboard != rules['DEFAULT_FOLDER']:
        keyboard = rules['DEFAULT_FOLDER']
        rules = parse_rules_mk_file(cur_dir / keyboard / 'rules.mk')
    return keyboard


def config_h(keyboard):
    """Parses all the config.h files for a keyboard.

    Args:
        keyboard: name of the keyboard

    Returns:
        a dictionary representing the content of the entire config.h tree for a keyboard
    """
    config = {}
    cur_dir = Path('keyboards')
    keyboard = Path(resolve_keyboard(keyboard))

    for dir in keyboard.parts:
        cur_dir = cur_dir / dir
        config = {**config, **parse_config_h_file(cur_dir / 'config.h')}

    return config


def rules_mk(keyboard):
    """Get a rules.mk for a keyboard

    Args:
        keyboard: name of the keyboard

    Returns:
        a dictionary representing the content of the entire rules.mk tree for a keyboard
    """
    cur_dir = Path('keyboards')
    keyboard = Path(resolve_keyboard(keyboard))
    rules = parse_rules_mk_file(cur_dir / keyboard / 'rules.mk')

    for i, dir in enumerate(keyboard.parts):
        cur_dir = cur_dir / dir
        rules = parse_rules_mk_file(cur_dir / 'rules.mk', rules)

    return rules

def render_layer_keymap(layout_data, render_ascii, layout_name="matrix", pins=None, is_split=False, no_rows=0):

    labels = get_key_labels()
    for i, layer in enumerate(labels):
        ln = "keymap-"+str(i)
        for x,y in zip(layout_data, layer):
           x['label']  = y[0]
        _ = render_layout(layout_data, render_ascii, layout_name=ln, pins=pins, is_split=is_split, no_rows=no_rows)


def render_layout(layout_data, render_ascii, key_labels=None, layout_name="matrix", pins=None, is_split=False, no_rows=0):
    """Renders a single layout.
    """
    textpad = [array('u', ' ' * 200) for x in range(100)]
    style = 'ascii' if render_ascii else 'unicode'

    rows = defaultdict(list)
    cols = defaultdict(list)
    circles = []
    rectangles = []
    iso = []
    bae = []
    labels = []
    rlabels = {}
    clabels = {}
    tooltips = []
    _xmin = _ymin = 1000000
    _xmax = _ymax = 0
    if is_split and no_rows == 0:
        # we come from somewhere we did not know the number of rows per side.
        no_rows = (max([key['matrix'][0] for key in layout_data])+1)//2
        print(no_rows)
    for key in layout_data:
        x = key.get('x', 0)
        y = key.get('y', 0)
        w = key.get('w', 1)
        h = key.get('h', 1)
        _xmin = min(x,_xmin)
        _ymin = min(y,_ymin)
        _xmax = max(x+w,_xmax)
        _ymax = max(y+h,_ymax)
        cr = key['matrix'][0]
        cc = key['matrix'][1]
        cx = x*swx + w*swx/2.0
        cy = y*swy + h*swy/2.0
        circles += [(cx,cy)]
        split = 'left' if is_split and cr < no_rows else 'right'
        if cr not in rlabels:
             rlabels[cr] = cy
        if cc not in clabels:
            clabels[cc] = cx

        rows[cr].append(cx+rox)
        rows[cr].append(cy+roy)
        lab=f"{ROW_LETTERS[cr]}-{COL_LETTERS[cc]}"
        lpins=''
        if pins:
            if is_split:
                if cr >= no_rows:
                     # we are on the right side...
                   if 'right' in pins:
                       if 'cols' in pins:
                          lpins += f"Col - {pins['right']['cols'][cc]}"
                       if 'rows' in pins:
                          lpins += f"\nRow - {pins['right']['rows'][cr-no_rows]}"
                   else:
                       if 'cols' in pins:
                          lpins += f"Col - {pins['cols'][cc]}"
                       if 'rows' in pins:
                          lpins += f"\nRow - {pins['rows'][cr-no_rows]}"
                else:
                   if 'cols' in pins:
                       lpins += f"Col - {pins['cols'][cc]}"
                   if 'rows' in pins:
                       lpins += f"\nRow - {pins['rows'][cr]}"
            else:
                if 'cols' in pins:
                    lpins += f"Col - {pins['cols'][cc]}"
                if 'rows' in pins:
                   lpins += f"\nRow - {pins['rows'][cr]}"
        if is_split:
            if cr<no_rows:
                split='left'
                tooltips.append(f"left side\nmatrix {cr}-{cc}\n       {lab}\n{lpins}")
            else:
                split='right'
                tooltips.append(f"right side\nmatrix {cr-no_rows}-{cc}\n       {lab}\n{lpins}")
        else:
            tooltips.append(f"\nmatrix {cr}-{cc}\n       {lab}\n{lpins}")

        if is_split:
            cols[cc,split].append(cx+cox)
            cols[cc,split].append(cy+coy)
        else:
            cols[cc].append(cx+cox)
            cols[cc].append(cy+coy)

        if key_labels:
            label = key_labels.pop(0)
            if label.startswith('KC_'):
                label = label[3:]
        else:
            label = key.get('label', '')
        labels.append(label)
        if 'encoder' in key:
            render_encoder(textpad, x, y, w, h, label, style)
        elif x >= 0.25 and w == 1.25 and h == 2:
            render_key_isoenter(textpad, x, y, w, h, label, style)
            iso.append(isoenter_key(x,y,w,h,swx,swy,rad))
        elif w == 1.5 and h == 2:
            render_key_baenter(textpad, x, y, w, h, label, style)
            bae.append(baeenter_key(x,y,w,h,swx,swy,rad))
        else:
            render_key_rect(textpad, x, y, w, h, label, style)
            rectangles += [(x*swx,y*swy,w*swx,h*swy)]

    canvas_width = (_xmax -_xmin) * swx + 5
    canvas_height = (_ymax - _ymin) * swy + 5


#    for r in rlabels:
#        d.append(draw.Text(ROW_LETTERS[r],fs,-5.0,rlabels[r],fill='black',center=True))
#
#    for c in clabels:
#        d.append(draw.Text(str(COL_LETTERS[c]),fs,clabels[c],-5.0,fill='black',center=True))
#    d.save_svg(f'{layout_name}.svg')
#    d.save_png(f'{layout_name}.png')
    draw_kb(canvas_width,canvas_height,keys=rectangles, centers=circles, labels=labels, iso=iso,
            bae=bae,rows=rows,cols=cols,tooltips=tooltips,svg=f'{layout_name}.svg',png=f'{layout_name}.png')
    lines = []
    for line in textpad:
        if line.tounicode().strip():
            lines.append(line.tounicode().rstrip())


    return '\n'.join(lines)


def render_layouts(info_json, render_ascii):
    """Renders all the layouts from an `info_json` structure.
    """
    layouts={}

    is_split = False
    if 'split' in info_json:
        is_split = info_json['split']['enabled']
    pins = None
    if 'matrix_pins' in info_json:
        pins = info_json['matrix_pins']
        if is_split and 'matrix_pins' in info_json['split']:
            pins['right']= info_json['split']['matrix_pins']['right']
    for layout in info_json['layouts']:
        layout_data = info_json['layouts'][layout]['layout']
        layouts[layout] = render_layout(layout_data, render_ascii, layout_name=layout,pins=pins,is_split=is_split)
        _ = render_layer_keymap(layout_data, render_ascii, layout_name=layout,pins=pins,is_split=is_split)

    return layouts


def render_key_rect(textpad, x, y, w, h, label, style):
    box_chars = BOX_DRAWING_CHARACTERS[style]
    x = ceil(x * 4)
    y = ceil(y * 3)
    w = ceil(w * 4)
    h = ceil(h * 3)

    label_len = w - 2
    label_leftover = label_len - len(label)

    if len(label) > label_len:
        label = label[:label_len]

    label_blank = ' ' * label_len
    label_border = box_chars['h'] * label_len
    label_middle = label + ' ' * label_leftover

    top_line = array('u', box_chars['tl'] + label_border + box_chars['tr'])
    lab_line = array('u', box_chars['v'] + label_middle + box_chars['v'])
    mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
    bot_line = array('u', box_chars['bl'] + label_border + box_chars['br'])

    textpad[y][x:x + w] = top_line
    textpad[y + 1][x:x + w] = lab_line
    for i in range(h - 3):
        textpad[y + i + 2][x:x + w] = mid_line
    textpad[y + h - 1][x:x + w] = bot_line


def render_key_isoenter(textpad, x, y, w, h, label, style):
    box_chars = BOX_DRAWING_CHARACTERS[style]
    x = ceil(x * 4)
    y = ceil(y * 3)
    w = ceil(w * 4)
    h = ceil(h * 3)

    label_len = w - 1
    label_leftover = label_len - len(label)

    if len(label) > label_len:
        label = label[:label_len]

    label_blank = ' ' * (label_len - 1)
    label_border_top = box_chars['h'] * label_len
    label_border_bottom = box_chars['h'] * (label_len - 1)
    label_middle = label + ' ' * label_leftover

    top_line = array('u', box_chars['tl'] + label_border_top + box_chars['tr'])
    lab_line = array('u', box_chars['v'] + label_middle + box_chars['v'])
    crn_line = array('u', box_chars['bl'] + box_chars['tr'] + label_blank + box_chars['v'])
    mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
    bot_line = array('u', box_chars['bl'] + label_border_bottom + box_chars['br'])

    textpad[y][x - 1:x + w] = top_line
    textpad[y + 1][x - 1:x + w] = lab_line
    textpad[y + 2][x - 1:x + w] = crn_line
    textpad[y + 3][x:x + w] = mid_line
    textpad[y + 4][x:x + w] = mid_line
    textpad[y + 5][x:x + w] = bot_line


def render_key_baenter(textpad, x, y, w, h, label, style):
    box_chars = BOX_DRAWING_CHARACTERS[style]
    x = ceil(x * 4)
    y = ceil(y * 3)
    w = ceil(w * 4)
    h = ceil(h * 3)

    label_len = w + 1
    label_leftover = label_len - len(label)

    if len(label) > label_len:
        label = label[:label_len]

    label_blank = ' ' * (label_len - 3)
    label_border_top = box_chars['h'] * (label_len - 3)
    label_border_bottom = box_chars['h'] * label_len
    label_middle = label + ' ' * label_leftover

    top_line = array('u', box_chars['tl'] + label_border_top + box_chars['tr'])
    mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
    crn_line = array('u', box_chars['tl'] + box_chars['h'] + box_chars['h'] + box_chars['br'] + label_blank + box_chars['v'])
    lab_line = array('u', box_chars['v'] + label_middle + box_chars['v'])
    bot_line = array('u', box_chars['bl'] + label_border_bottom + box_chars['br'])

    textpad[y][x:x + w] = top_line
    textpad[y + 1][x:x + w] = mid_line
    textpad[y + 2][x:x + w] = mid_line
    textpad[y + 3][x - 3:x + w] = crn_line
    textpad[y + 4][x - 3:x + w] = lab_line
    textpad[y + 5][x - 3:x + w] = bot_line


def render_encoder(textpad, x, y, w, h, label, style):
    box_chars = ENC_DRAWING_CHARACTERS[style]
    x = ceil(x * 4)
    y = ceil(y * 3)
    w = ceil(w * 4)
    h = ceil(h * 3)

    label_len = w - 2
    label_leftover = label_len - len(label)

    if len(label) > label_len:
        label = label[:label_len]

    label_blank = ' ' * label_len
    label_border = box_chars['h'] * label_len
    label_middle = label + ' ' * label_leftover

    top_line = array('u', box_chars['tl'] + label_border + box_chars['tr'])
    lab_line = array('u', box_chars['vl'] + label_middle + box_chars['vr'])
    mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
    bot_line = array('u', box_chars['bl'] + label_border + box_chars['br'])

    textpad[y][x:x + w] = top_line
    textpad[y + 1][x:x + w] = lab_line
    for i in range(h - 3):
        textpad[y + i + 2][x:x + w] = mid_line
    textpad[y + h - 1][x:x + w] = bot_line
