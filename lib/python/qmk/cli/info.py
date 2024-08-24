"""Keyboard information script.

Compile an info.json for a particular keyboard and pretty-print it.
"""
import sys
import os
import json

from milc import cli

from qmk.json_encoders import InfoJSONEncoder
from qmk.constants import COL_LETTERS, ROW_LETTERS
from qmk.decorators import automagic_keyboard, automagic_keymap
from qmk.keyboard import keyboard_completer, keyboard_folder, render_layouts, render_layout, rules_mk
from qmk.info import info_json, keymap_json
from qmk.keymap import locate_keymap, c2json
from qmk.path import is_keyboard
from layers import get_key_labels
from draw_keyboard import summary_keymap

UNICODE_SUPPORT = sys.stdout.encoding.lower().startswith('utf')


def _strip_api_content(info_json):
    # Ideally this would only be added in the API pathway.
    info_json.pop('platform', None)
    info_json.pop('platform_key', None)
    info_json.pop('processor_type', None)
    info_json.pop('protocol', None)
    info_json.pop('config_h_features', None)
    info_json.pop('keymaps', None)
    info_json.pop('keyboard_folder', None)
    info_json.pop('parse_errors', None)
    info_json.pop('parse_warnings', None)

    for layout in info_json.get('layouts', {}).values():
        layout.pop('filename', None)
        layout.pop('c_macro', None)
        layout.pop('json_layout', None)

    if 'matrix_pins' in info_json:
        info_json.pop('matrix_size', None)

    for feature in ['rgb_matrix', 'led_matrix']:
        if info_json.get(feature, {}).get("layout", None):
            info_json[feature].pop('led_count', None)

    return info_json

def is_split(kb_info_json):
    is_split = False
    if 'split' in kb_info_json:
        is_split = kb_info_json['split']['enabled']
    return is_split

def get_pins(kb_info_json):

    pins = None
    if 'matrix_pins' in kb_info_json:
        pins = kb_info_json['matrix_pins']
        if is_split(kb_info_json) and 'matrix_pins' in kb_info_json['split']:
            pins['right']= kb_info_json['split']['matrix_pins']['right']
    return pins

def show_keymap(kb_info_json, title_caps=True):
    """Render the keymap in ascii art.
    """
    keymap_path = locate_keymap(cli.config.info.keyboard, cli.config.info.keymap)
    split = is_split(kb_info_json)
    pins = get_pins(kb_info_json)

    print(f"show keymap {keymap_path}")
    if keymap_path:
        if keymap_path.suffix == '.json':
            keymap_data = json.load(keymap_path.open(encoding='utf-8'))
        else:
            try:
                keymap_data = c2json(cli.config.info.keyboard, cli.config.info.keymap, keymap_path, use_cpp=True)
            except:
                print(f"cpp disabled in reading {keymap_path}")
                keymap_data = c2json(cli.config.info.keyboard, cli.config.info.keymap, keymap_path, use_cpp=False)

        layout_name = keymap_data['layout']
        layout_name = kb_info_json.get('layout_aliases', {}).get(layout_name, layout_name)  # Resolve alias names
        print(f"{layout_name=}")
        labels, keycodes, _  = get_key_labels(keymap_data,keymap_path)
        keyb = {}
        for layer_num, layer in enumerate(keymap_data['layers']):
            if title_caps:
                cli.echo('{fg_cyan}Keymap %s Layer %s{fg_reset}:', cli.config.info.keymap, layer_num)
            else:
                cli.echo('{fg_cyan}keymap.%s.layer.%s{fg_reset}:', cli.config.info.keymap, layer_num)
            labs = []
            for x,y in zip(kb_info_json['layouts'][layout_name]['layout'], labels[layer_num]):
                labs.append(y[0] if y else '')
            fk_name = "-".join(cli.config.info.keyboard.split(os.sep))
            print(render_layout(kb_info_json['layouts'][layout_name]['layout'], cli.config.info.ascii,
                                key_labels = labs,
                                keycodes = keycodes[layer_num],layout_name="-".join([fk_name,layout_name,cli.config.info.keymap,str(layer_num)]),
                                is_split=split, pins=pins, kb=keyb))
        name = "_".join(cli.config.info.keyboard.split("/"))+"_"+cli.config.info.keymap
        summary_keymap(name,keyb)

def show_layouts(kb_info_json, title_caps=True):
    """Render the layouts with info.json labels.
    """
    fk_name = "-".join(cli.config.info.keyboard.split(os.sep))
    for layout_name, layout_art in render_layouts(kb_info_json, cli.config.info.ascii,fk_name).items():
        title = f'Layout {layout_name.title()}' if title_caps else f'layouts.{layout_name}'
        cli.echo('{fg_cyan}%s{fg_reset}:', title)
        print(layout_art)  # Avoid passing dirty data to cli.echo()

def has_rgb(kb_info_json_rgb_matrix,nr,nc):
   for led in kb_info_json_rgb_matrix['layout']:
       if 'matrix' in led:
           x = led['matrix'][0]
           y = led['matrix'][1]
           if (nr,nc) == (x,y):
              try:
                  return True, led['x'], led['y'], led['flags']
              except:
                  return True, led['x'], led['y'], 0
   return False, None, None, None

def show_matrix(kb_info_json, title_caps=True):
    """Render the layout with matrix labels in ascii art.
    """
    split = is_split(kb_info_json)
    pins = get_pins(kb_info_json)
    for layout_name, layout in kb_info_json['layouts'].items():
        # Build our label list
        labels = []
        rgb_layout = {}
        rgb_back = []
        for key in layout['layout']:
            if 'matrix' in key:
                nr = key['matrix'][0]
                nc = key['matrix'][1]
                row = ROW_LETTERS[nr]
                col = COL_LETTERS[nc]
                try:
                    rgb, x,y,flag = has_rgb(kb_info_json['rgb_matrix'],nr,nc)
                    if rgb:
                        rgb_layout[(nr,nc)] = {'x':x,'y':y,'flags':flag}
                except:
                    pass
                labels.append(f"{row+col}\n  {nr},{nc}")
            else:
                labels.append('')
        try:
            for led in kb_info_json['rgb_matrix']['layout']:
                try:
                    x = led['matrix'][0]
                except:
                    rgb_back += [(led['x'],led['y'],led['flags'])]
        except:
            pass
        # Print the header
        if title_caps:
            cli.echo('{fg_blue}Matrix for "%s"{fg_reset}:', layout_name)
        else:
            cli.echo('{fg_blue}matrix_%s{fg_reset}:', layout_name)

        fk_name = "-".join(cli.config.info.keyboard.split(os.sep))
        ln = "-".join([fk_name,'matrix',layout_name])
        print(render_layout(kb_info_json['layouts'][layout_name]['layout'], cli.config.info.ascii,
                            key_labels=labels,is_split=split,layout_name=ln, pins=pins, show_wires=True,
                            show_rgb=True,rgb_layout=rgb_layout,rgb_back=rgb_back))


def print_friendly_output(kb_info_json):
    """Print the info.json in a friendly text format.
    """
    cli.echo('{fg_blue}Keyboard Name{fg_reset}: %s', kb_info_json.get('keyboard_name', 'Unknown'))
    cli.echo('{fg_blue}Manufacturer{fg_reset}: %s', kb_info_json.get('manufacturer', 'Unknown'))
    if 'url' in kb_info_json:
        cli.echo('{fg_blue}Website{fg_reset}: %s', kb_info_json.get('url', ''))
    if kb_info_json.get('maintainer', 'qmk') == 'qmk':
        cli.echo('{fg_blue}Maintainer{fg_reset}: QMK Community')
    else:
        cli.echo('{fg_blue}Maintainer{fg_reset}: %s', kb_info_json['maintainer'])
    cli.echo('{fg_blue}Layouts{fg_reset}: %s', ', '.join(sorted(kb_info_json['layouts'].keys())))
    cli.echo('{fg_blue}Processor{fg_reset}: %s', kb_info_json.get('processor', 'Unknown'))
    cli.echo('{fg_blue}Bootloader{fg_reset}: %s', kb_info_json.get('bootloader', 'Unknown'))
    if 'layout_aliases' in kb_info_json:
        aliases = [f'{key}={value}' for key, value in kb_info_json['layout_aliases'].items()]
        cli.echo('{fg_blue}Layout aliases:{fg_reset} %s' % (', '.join(aliases),))


def print_text_output(kb_info_json):
    """Print the info.json in a plain text format.
    """
    for key in sorted(kb_info_json):
        if key == 'layouts':
            cli.echo('{fg_blue}layouts{fg_reset}: %s', ', '.join(sorted(kb_info_json['layouts'].keys())))
        else:
            cli.echo('{fg_blue}%s{fg_reset}: %s', key, kb_info_json[key])

    if cli.config.info.layouts:
        show_layouts(kb_info_json, False)

    if cli.config.info.matrix:
        show_matrix(kb_info_json, False)

    if cli.config_source.info.keymap and cli.config_source.info.keymap != 'config_file':
        show_keymap(kb_info_json, False)


def print_dotted_output(kb_info_json, prefix=''):
    """Print the info.json in a plain text format with dot-joined keys.
    """
    for key in sorted(kb_info_json):
        new_prefix = f'{prefix}.{key}' if prefix else key

        if key in ['parse_errors', 'parse_warnings']:
            continue
        elif key == 'layouts' and prefix == '':
            cli.echo('{fg_blue}layouts{fg_reset}: %s', ', '.join(sorted(kb_info_json['layouts'].keys())))
        elif isinstance(kb_info_json[key], dict):
            print_dotted_output(kb_info_json[key], new_prefix)
        elif isinstance(kb_info_json[key], list):
            cli.echo('{fg_blue}%s{fg_reset}: %s', new_prefix, ', '.join(map(str, sorted(kb_info_json[key]))))
        else:
            cli.echo('{fg_blue}%s{fg_reset}: %s', new_prefix, kb_info_json[key])


def print_parsed_rules_mk(keyboard_name):
    rules = rules_mk(keyboard_name)
    for k in sorted(rules.keys()):
        print('%s = %s' % (k, rules[k]))
    return


@cli.argument('-kb', '--keyboard', type=keyboard_folder, completer=keyboard_completer, help='Keyboard to show info for.')
@cli.argument('-km', '--keymap', help='Keymap to show info for (Optional).')
@cli.argument('-l', '--layouts', action='store_true', help='Render the layouts.')
@cli.argument('-m', '--matrix', action='store_true', help='Render the layouts with matrix information.')
@cli.argument('-f', '--format', default='friendly', arg_only=True, help='Format to display the data in (friendly, text, json) (Default: friendly).')
@cli.argument('--ascii', action='store_true', default=not UNICODE_SUPPORT, help='Render layout box drawings in ASCII only.')
@cli.argument('-r', '--rules-mk', action='store_true', help='Render the parsed values of the keyboard\'s rules.mk file.')
@cli.argument('-a', '--api', action='store_true', help='Show fully processed info intended for API consumption.')
@cli.subcommand('Keyboard information.')
@automagic_keyboard
@automagic_keymap
def info(cli):
    """Compile an info.json for a particular keyboard and pretty-print it.
    """
    # Determine our keyboard(s)
    if not cli.config.info.keyboard:
        cli.log.error('Missing parameter: --keyboard')
        cli.subcommands['info'].print_help()
        return False

    if not is_keyboard(cli.config.info.keyboard):
        cli.log.error('Invalid keyboard: "%s"', cli.config.info.keyboard)
        return False

    if bool(cli.args.rules_mk):
        print_parsed_rules_mk(cli.config.info.keyboard)
        return False

    # default keymap stored in config file should be ignored
    if cli.config_source.info.keymap == 'config_file':
        cli.config_source.info.keymap = None

    # Build the info.json file
    if cli.config.info.keymap:
        kb_info_json = keymap_json(cli.config.info.keyboard, cli.config.info.keymap)
    else:
        kb_info_json = info_json(cli.config.info.keyboard)

    if not cli.args.api:
        kb_info_json = _strip_api_content(kb_info_json)

    # Output in the requested format
    if cli.args.format == 'json':
        print(json.dumps(kb_info_json, cls=InfoJSONEncoder, sort_keys=True))
        return True
    elif cli.args.format == 'text':
        print_dotted_output(kb_info_json)
        title_caps = False
    elif cli.args.format == 'friendly':
        print_friendly_output(kb_info_json)
        title_caps = True
    else:
        cli.log.error('Unknown format: %s', cli.args.format)
        return False

    # Output requested extras
    if cli.config.info.layouts:
        show_layouts(kb_info_json, title_caps)

    if cli.config.info.matrix:
        show_matrix(kb_info_json, title_caps)

    if cli.config.info.keymap:
        show_keymap(kb_info_json, title_caps)
