"""Convert raw KLE to JSON
"""
import json
import os
from pathlib import Path

from argcomplete.completers import FilesCompleter
from milc import cli
from kle2xy import KLE2xy

from qmk.converter import kle2qmk
from qmk.json_encoders import InfoJSONEncoder


from decimal import Decimal
from collections import defaultdict
from draw_keyboard import draw_kb,rad,swx,swy,rox,roy,cox,coy,isoenter_key,baeenter_key

@cli.argument('filename', completer=FilesCompleter('.json'), help='The KLE raw txt to convert')
@cli.argument('-f', '--force', action='store_true', help='Flag to overwrite current info.json')
@cli.subcommand('Convert a KLE layout to a Configurator JSON', hidden=False if cli.config.user.developer else True)
def kle2json(cli):
    """Convert a KLE layout to QMK's layout format.
    """  # If filename is a path
    if cli.args.filename.startswith("/") or cli.args.filename.startswith("./"):
        file_path = Path(cli.args.filename)
    # Otherwise assume it is a file name
    else:
        file_path = Path(os.environ['ORIG_CWD'], cli.args.filename)
    # Check for valid file_path for more graceful failure
    if not file_path.exists():
        cli.log.error('File {fg_cyan}%s{style_reset_all} was not found.', file_path)
        return False
    out_path = file_path.parent
    raw_code = file_path.read_text(encoding='utf-8')
    # Check if info.json exists, allow overwrite with force
    if Path(out_path, "info.json").exists() and not cli.args.force:
        cli.log.error('File {fg_cyan}%s/info.json{style_reset_all} already exists, use -f or --force to overwrite.', out_path)
        return False
    try:
        # Convert KLE raw to x/y coordinates (using kle2xy package from skullydazed)
        kle = KLE2xy(raw_code)
    except Exception as e:
        cli.log.error('Could not parse KLE raw data: %s', raw_code)
        cli.log.exception(e)
        return False
    keyboard = {
        'keyboard_name': "somenameforkeyboard" if kle.name == '' else kle.name,
        'url': '',
        'maintainer': 'qmk',
        'matrix_pins':{
             'cols': ["NO_PIN"]*(int(kle.columns)+1),
             'rows': ["NO_PIN"]*int(kle.rows)
            },
        'layouts': {
            'LAYOUT': {
                'layout': kle2qmk(kle)
            }
        },
    }
    filename = keyboard['keyboard_name']
    # Write our info.json
    keyboard = json.dumps(keyboard, indent=4, separators=(', ', ': '), sort_keys=False, cls=InfoJSONEncoder)
    info_json_file = out_path / 'info.json'
    info_json_file.write_text(keyboard)
    cli.log.info('Wrote out {fg_cyan}%s/info.json', out_path)

    rows =defaultdict(list)
    cols =defaultdict(list)
    circles = []
    rectangles=[]
    labels = []
    tooltips = []
    iso = []
    bae = []
    _xmin = 100000000000
    _ymin = 100000000000
    _ymax = -1000000000
    _xmax = -1000000000
    for row_num,row in enumerate(kle):
        for key in row:
            krow=key['matrix'][0]
            kcol=key['matrix'][1]
            center = (key['x'],abs(key['y']))
            xmin = key['xmin']
            xmax = key['xmax']
            ymin = abs(key['ymin'])
            ymax = abs(key['ymax'])
            _xmin = min(_xmin,xmin)
            _ymin = min(_ymin, ymin)
            _xmax = max(_xmax,xmax)
            _ymax = max(_ymax,ymax)
            width = (xmax - xmin)
            height = abs(ymax - ymin)
            if 'isoenter' in key:
                _w = float(width)/swx
                _h = float(height)/swy
                _x = float(xmin)/swx
                _y = float(abs(ymin))/swy
                iso.append(isoenter_key(_x,_y,_w,_h,swx,swy,rad))
            if 'baeenter' in key:
                _w = float(width)/swx
                _h = float(height)/swy
                _x = float(xmin)/swx
                _y = float(abs(ymin))/swy
                bae.append(baeenter_key(_x,_y,_w,_h,swx,swy,rad))
            else:
                rectangles += [(xmin,abs(ymin),width,height)]
            rows[krow].append(center[0] + rox)
            rows[krow].append(center[1] + roy)
            cols[kcol].append(center[0] + cox)
            cols[kcol].append(center[1] + coy)
            labels.append(key['name'])
            tooltips.append("matrix: "+str(krow)+"-"+str(kcol))
            circles += [(center[0],center[1])]

    width = abs(_xmax - _xmin) + 5
    height = abs(_ymax - _ymin) + 5
    draw_kb(width,height,keys=rectangles, centers=circles, labels=labels,iso=iso,bae=bae,
            rows=rows,cols=cols,tooltips=tooltips,svg=f'{filename}.svg',png=f'{filename}.png')

