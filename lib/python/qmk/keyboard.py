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
import unicodedata

BOX_DRAWING_CHARACTERS = {
    "unicode": {
        "tl": "┌",
        "tr": "┐",
        "tm": "┬",
        "bl": "└",
        "bm": "┴",
        "br": "┘",
        "ml": "├",
        "mr": "┤",
        "mm":"┼",
        "v": "│",
        "h": "─"
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

        current_path = resolve_keyboard(current_path)

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
    """Determine the keyboard name by stripping off the base_path and filename.
    """
    return path.replace(base_path, "").rsplit(os.path.sep, 1)[0]


def keyboard_completer(prefix, action, parser, parsed_args):
    """Returns a list of keyboards for tab completion.
    """
    return list_keyboards()


def list_keyboards(resolve_defaults=True):
    """Returns a list of all keyboards - optionally processing any DEFAULT_FOLDER.
    """
    # We avoid pathlib here because this is performance critical code.
    paths = []
    for marker in ['rules.mk', 'keyboard.json']:
        kb_wildcard = os.path.join(base_path, "**", marker)
        paths += [path for path in glob(kb_wildcard, recursive=True) if os.path.sep + 'keymaps' + os.path.sep not in path]

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

def render_layout(layout_data, render_ascii, key_labels=None, layout_name="somedefault", keycodes=None, pins=None, is_split=False,
                  show_wires=False,show_rgb=False,rgb_layout=None,rgb_back=None, kb=None):
    """Renders a single layout.
    """
    textpad = [array('u', ' ' * 200) for x in range(100)]
    style = 'ascii' if render_ascii else 'unicode'

    rows = defaultdict(list)
    cols = defaultdict(list)
    circles = []
    rgbs = []
    rectangles = []
    iso = []
    bae = []
    labels = []
    tooltips = []
    encoders = []
    backlights = []
    _xmin = _ymin = 1000000
    _xmax = _ymax = 0
    mrows = max([key['matrix'][0] for key in layout_data])
    if is_split:
        # we come from somewhere we did not know the number of rows per side.
        no_rows = (mrows+1)//2
    last_col_row = []
    for r in range(mrows+1):
        last_col_row.append(max([key['matrix'][1] for key in layout_data if key['matrix'][0] == r and 'encoder' not in key ]))
    extra = False
    ccr = 0
    rcr = 0
    sp = ''
    if show_rgb:
        for b in rgb_back:
            backlights += [(b[0]*swx,b[1]*swy,b[2])]

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

        if key_labels:
            label = key_labels.pop(0)
        else:
            label = key.get('label', '')

        if keycodes:
            tlabel = "  code " + keycodes.pop(0)
        else:
            tlabel = "       "

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
                          lpins += f"   col {pins['right']['cols'][cc]}"
                       if 'rows' in pins:
                          lpins += f"\n   row {pins['right']['rows'][cr-no_rows]}"
                   else:
                       if 'cols' in pins:
                          lpins += f"   col {pins['cols'][cc]}"
                       if 'rows' in pins:
                          lpins += f"\n   row {pins['rows'][cr-no_rows]}"
                else:
                   if 'cols' in pins:
                       lpins += f"   col {pins['cols'][cc]}"
                   if 'rows' in pins:
                       lpins += f"\n   row {pins['rows'][cr]}"
            else:
                if 'cols' in pins:
                    lpins += f"   col {pins['cols'][cc]}"
                if 'rows' in pins:
                   lpins += f"\n   row {pins['rows'][cr]}"

        if is_split:
            if cr<no_rows:
                split='left'
                tooltips.append(f"left side\n{tlabel}\nmatrix ({cr},{cc})\n       {lab}\n{lpins}")
            else:
                split='right'
                tooltips.append(f"right side\n{tlabel}\nmatrix ({cr-no_rows},{cc})\n       {lab}\n{lpins}")
        else:
            tooltips.append(f"\n{tlabel}\nmatrix ({cr},{cc})\n       {lab}\n{lpins}")

        if is_split:
            cols[cc,split].append(cx+cox)
            cols[cc,split].append(cy+coy)
        else:
            cols[cc].append(cx+cox)
            cols[cc].append(cy+coy)


        labels.append(label)
        label = ''.join(c for c in label.replace('\n','') if unicodedata.east_asian_width(c) != 'W')
        if len(label)>4:
            label=label.replace('\n','')[0:4]
        if show_rgb:
            try:
                flag = rgb_layout[(cr,cc)]['flags']
                rgbs += [(x*swx + w*swx-rad,y*swy + h*swy-rad,flag)]
                label = label.strip()+'☼'
            except:
                pass

        if 'encoder' in key:
            render_encoder(textpad, x, y, w, h, label, style)
            encoders += [(x*swx+w*swx/2.0,y*swy+h*swy/2.0,h*swy/2.0)]
        elif x >= 0.25 and w == 1.25 and h == 2:
            render_key_isoenter(textpad, x, y, w, h, label, style)
            iso.append(isoenter_key(x,y,w,h,swx,swy,rad))
        elif w == 1.5 and h == 2:
            render_key_baenter(textpad, x, y, w, h, label, style)
            bae.append(baeenter_key(x,y,w,h,swx,swy,rad))
        else:

            if layout_name.startswith('mlego') or layout_name.startswith('tipro'):
                if cr ==0:
                    position = 't'
                elif cr == mrows:
                    position = 'b'
                else:
                    position = 'm'
                if cc == 0:
                    position = 'l'+position
                elif cc == last_col_row[cr] :
                    position = 'r'+position
                    try:
                        if last_col_row[cr] != last_col_row[cr+1]:

                            sp = 'b'+position
                            extra = True
                            rcr = cr + 1
                            ccr = last_col_row[cr+1]
                    except:
                        pass

                render_key(textpad, x, y, w, h, label, style,position)
                if extra and rcr == cr and cc == ccr:
                    extra = False
                    render_key(textpad, x+1, y, w, h, label, style, sp)
            else:
                render_key_rect(textpad, x, y, w, h, label, style)

            rectangles += [(x*swx,y*swy,w*swx,h*swy)]

    canvas_width = (_xmax -_xmin) * swx + 5
    canvas_height = (_ymax - _ymin) * swy + 5

    if not show_wires:
        rows = None
        cols = None

    draw_kb(canvas_width,canvas_height,keys=rectangles, encoders=encoders, centers=circles, labels=labels, iso=iso,
            bae=bae,rows=rows,cols=cols,tooltips=tooltips,svg=f'{layout_name}.svg',rgbs=rgbs if show_rgb else None,
            backlights=backlights if show_rgb else None)
    if kb is not None:
      q = {}
      q['width'] = canvas_width
      q['height'] = canvas_height
      q['keys'] = rectangles
      q['encoders'] = encoders
      q['labels'] = labels
      q['centers'] = circles
      q['iso'] = iso
      q['bae'] = bae
      q['rows'] = rows
      q['cols'] = cols
      q['rgbs'] = rgbs
      q['backlights'] = backlights
      kb[layout_name] = q
    lines = []
    for line in textpad:
        if line.tounicode().strip():
            lines.append(line.tounicode().rstrip())

    return '\n'.join(lines)


def render_layouts(info_json, render_ascii,kb_name=None):
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
        ln = "-".join([kb_name,layout])
        layout_data = info_json['layouts'][layout]['layout']
        layouts[layout] = render_layout(layout_data, render_ascii, layout_name=ln,pins=pins,is_split=is_split)
    return layouts

def render_key(textpad, x, y, w, h, label, style,position='m'):
    box_chars = BOX_DRAWING_CHARACTERS[style]
    x = ceil(x * 5)
    y = ceil(y * 3)
    w = ceil(w * 5)
    h = ceil(h * 3)

    top_line = bot_line = lab_line = mid_line = None

    label_len = w - 1
    label_leftover = label_len - len(label)

    if len(label) > label_len:
        label = label[:label_len]

    label_blank = ' ' * label_len
    label_border = box_chars['h'] * label_len
    label_middle = label + ' ' * label_leftover
    if len(label) == 1:
        label_middle = ' ' + label + ' '*2
    if len(label) == 2:
        label_middle = ' '+ label + ' '

    if position == 'm':
            top_line = array('u', box_chars['mm'] + label_border + box_chars['h'])
            lab_line = array('u', box_chars['v'] + label_middle)
            mid_line = array('u', box_chars['v'] + label_blank)
    elif position == 'rm':
            top_line = array('u', box_chars['mm'] + label_border + box_chars['mr'])
            lab_line = array('u', box_chars['v'] + label_middle  + box_chars['v'])
            mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
    elif position == 'lm':
            top_line = array('u', box_chars['ml'] + label_border + box_chars['h'])
            lab_line = array('u', box_chars['v'] + label_middle)
            mid_line = array('u', box_chars['v'] + label_blank)
    elif position == 't':
            top_line = array('u', box_chars['tm'] + label_border + box_chars['h'])
            lab_line = array('u', box_chars['v'] + label_middle)
            mid_line = array('u', box_chars['v'] + label_blank)
    elif position == 'rt':
            top_line = array('u', box_chars['tm'] + label_border + box_chars['tr'])
            lab_line = array('u', box_chars['v'] + label_middle + box_chars['v'] )
            mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
    elif position == 'brt':
            top_line = array('u', box_chars['mm'] + label_border + box_chars['br'])
    elif position == 'lt':
            top_line = array('u', box_chars['tl'] + label_border + box_chars['h'])
            lab_line = array('u', box_chars['v'] + label_middle)
            mid_line = array('u', box_chars['v'] + label_blank)
    elif position == 'b':
            top_line = array('u', box_chars['mm'] + label_border + box_chars['h'])
            lab_line = array('u', box_chars['v'] + label_middle)
            mid_line = array('u', box_chars['v'] + label_blank)
            bot_line = array('u', box_chars['bm'] + label_border + box_chars['h'])
    elif position == 'rb':
            top_line = array('u', box_chars['mm'] + label_border + box_chars['mr'])
            lab_line = array('u', box_chars['v'] + label_middle + box_chars['v'])
            mid_line = array('u', box_chars['v'] + label_blank + box_chars['v'])
            bot_line = array('u', box_chars['bm'] + label_border + box_chars['br'])
    elif position == 'lb':
            top_line = array('u', box_chars['ml'] + label_border + box_chars['h'])
            lab_line = array('u', box_chars['v'] + label_middle)
            mid_line = array('u', box_chars['v'] + label_blank)
            bot_line = array('u', box_chars['bl'] + label_border + box_chars['h'])

    if top_line:
        textpad[y][x:x + w] = top_line
    if lab_line:
        textpad[y + 1][x:x + w] = lab_line
    for i in range(h - 3):
        if mid_line:
            textpad[y + i + 2][x:x + w] = mid_line
    if bot_line:
        textpad[y + h - 1][x:x + w] = bot_line

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
    x = ceil(x * 5)
    y = ceil(y * 3)
    w = ceil(w * 5)
    h = ceil(h * 3)

    label_len = w - 1
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
