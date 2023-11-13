import hjson
from collections import defaultdict
import re
import codecs
from pathlib import Path



def get_key_labels(keymap, keymap_c):

    keycodes = hjson.load(open("data/constants/keycodes/extras/keycodes_uk_0.0.1.hjson"))
    basic = hjson.load(open("data/constants/keycodes/keycodes_0.0.1_basic.hjson"))
    rgb = hjson.load(open("data/constants/keycodes/keycodes_0.0.1_lighting.hjson"))
    quant = hjson.load(open("data/constants/keycodes/keycodes_0.0.1_quantum.hjson"))

    specials = {'SC_LSPO': ['⇧','('],'SC_RSPC':['⇧',')'],'TT(_LWR)':['⇓'],'TT(_RSE)':['⇑'],'G(KC_P)':['🖵'],'KC_PSCR':['🖶'],'A(KC_F2)':['⌕'],
                'KC_ESC':['⎋'],'KC_LSFT':['⇧'],'KC_RSFT':['⇧'],
                'KC_TAB':['↹'],'KC_LCTL':['⎈'],'KC_RCTL':['⎈'],'KC_RALT':['⎇'],'KC_LALT':['⎇'],'KC_BSPC':['⌫'],'KC_DEL':['⌦'],
                'KC_DOWN':['↓'],'KC_RGHT':['→'],'KC_UP':['↑'],'KC_LEFT':['←'],
                'KC_ENT':['⏎'],'KC_PENT':['⏎'],'KC_LGUI':[''],'KC_RGUI':[''],'KC_APP':[''],'KC_SPC':[' '],
                'KC_VOLU':['🕪'],'KC_VOLD':['🕩'],'KC_MPLY':['⏯'],'KC_MPRV':['⏮'],'KC_MNXT':['⏭'],'KC_MUTE':['🕨'],
                'KC_SLEP':['⏾'],'KC_WAKE':['⏽'],
                'KC_SCRL':['⇳'],'KC_PAUS':['⎉'],'KC_NUM':['⇭'],'KC_CAPS':['⇪'], 'KC_INS': ['Ins'],
                'KC_HOME':['⇱'],'KC_PGUP':['PgUp'],'KC_PGDN':['PgDn'],'KC_END':['⇲'],
                'KC_WH_L':['⇠','🖰'],'KC_WH_R':['🖰','⇢'],'KC_WH_U':['⇡','🖰'],'KC_WH_D':['🖰','⇣'],
                'KC_MS_L':['↞','🖰'],'KC_MS_R':['🖰','↠'],'KC_MS_U':['↟','🖰'],'KC_MS_D':['🖰','↡'],
                'KC_BTN1':['🖰','1'],'KC_BTN2':['🖰','2'],'KC_BTN3':['🖰','3'],'KC_BTN4':['🖰','4'],
                'RGB_TOG':['l','☀','☼'],'QK_BOOT':['b','♽'],'QK_RBT':['r','♲'],
                'UC_LINX':['u',''],'UC_WIN':['u',''],'UC_WINC':['u','❖'],'UC_MAC':['u',''],
                'UC_NEXT':['u','⎘'], 'UC_PREV':['u','⎗'],
                'RGB_RMOD':['l','⎘'], 'RGB_MOD':['l','⎗'],'DB_TOGG':['d','⚛'],'EE_CLR':['e','␥'],
                'RGB_ON_EFF':['l','☀'],'RGB_OFF_EFF':['l','☼'],'RGB_IND':['l','⌧'],'RGB_LSD': ['l','⏹'],'RGB_HEAT':['l','⌇']}
# in theory this can be provided by the user with the keymaps
    q = Path("specials.json")
    if q.exists():
        print(f"read user specified substitutions from {q}")
        specs = hjson.load(open(q))
        specials.update(specs)
# up codes⇪
    ups = {}
    if keymap_c.suffix == '.c':
        fup = open(keymap_c).read()

        x = re.findall("(?xs) unicode_map.+?{(.+?)}", fup)
        if x:
            for x in x[0].split("\n"):
                if len(x) > 0:
                    k = re.findall("\[(.+?)\]",x)[0]
                    v = re.findall("=(.+?),",x)[0]
                    ups[k]=v
    n = len(keymap['layers'])
    labels = [defaultdict(list) for i in range(n)]


    for layer in range(n):
        m = len(keymap['layers'][layer])
        #print(f"on {layer=} to find {m} labels")
        left=[]
        for key in keymap['layers'][layer]:

            label = [ v['label'].lower() for k,v in keycodes['aliases'].items() if v['key']==key ]
            if label:
                labels[layer][key] = label
            else:
                left.append(key)
    #print(f"on {layer=} found {len(labels[layer])} labels, left to find {len(left)} - UK keycodes")
        found = []
        for key in left:
            label = [ v for k,v in specials.items() if key == k ]
            if label:
                labels[layer][key]=label[0]
                found.append(key)
        left = [x for x in left if x not in found]
    #print(f"on {layer=} found {len(labels[layer])} labels, left to find {len(left)} - special keycodes")
        for ks in [basic]:
            found = []
            for key in left:
                label = [ v['label'].lower() for k,v in ks['keycodes'].items() if v['key']==key or ('aliases' in v and key in v['aliases']) ]
                if label:
                    labels[layer][key] = label
                    found.append(key)
            left = [x for x in left if x not in found]
    #    print(f"on {layer=} found {len(labels[layer])} labels, left to find {len(left)} - basic keycodes")
    # purge the UP
        found = []
        for key in left:
            if "UP(" in key:
                k = re.findall("\((.+?)\)",key)[0].split(",")
                l1 = ups[k[0]].strip(" ").replace('0x',r'\u')
                l2 = ups[k[1]].strip(" ").replace('0x',r'\u')
                labels[layer][key]=[codecs.unicode_escape_decode(l1)[0], codecs.unicode_escape_decode(l2)[0]]
                found.append(key)
        left = [x for x in left if x not in found]
    #print(f"on {layer=} found {len(labels[layer])} labels, left to find {len(left)} - ups")
        for ks in [rgb,quant]:
            found = []
            for key in left:
                label = [ key for k,v in ks['keycodes'].items() if v['key']==key or ('aliases' in v and key in v['aliases']) ]
                if label:
                    for i,l in enumerate(label):
                        if l.startswith('RGB_'):
                            label[i] = l[4:]
                        if l.startswith('QK_') or l.startswith('UC_'):
                            label[i] = l[3:]
                    labels[layer][key] = label
                    found.append(key)
            left = [x for x in left if x not in found]
     #   print(f"on {layer=} found {len(labels[layer])} labels, left to find {len(left)} - rgb/quant")

    o_labels = [ list() for i in range(n)]
    for layer in range(n):
        for key in keymap['layers'][layer]:
            l = labels[layer][key]
            x = l
            if len(l) > 1 :
                x = ["\n".join(l)]
            o_labels[layer].append(x)
    return o_labels, keymap['layers']
