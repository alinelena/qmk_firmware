"""Draw an svg or render png for layout or matrix
"""
import drawsvg as draw
from milc import cli
from pathlib import Path
from math import log
import math

css4_colours = ['aliceblue', 'antiquewhite', 'aqua', 'aquamarine', 'azure', 'beige', 'bisque', 'black', 'blanchedalmond', 'blue', 'blueviolet', 'brown', 'burlywood', 'cadetblue', 'chartreuse', 'chocolate', 'coral', 'cornflowerblue', 'cornsilk', 'crimson', 'cyan', 'darkblue', 'darkcyan', 'darkgoldenrod', 'darkgray', 'darkgreen', 'darkgrey', 'darkkhaki', 'darkmagenta', 'darkolivegreen', 'darkorange', 'darkorchid', 'darkred', 'darksalmon', 'darkseagreen', 'darkslateblue', 'darkslategray', 'darkslategrey', 'darkturquoise', 'darkviolet', 'deeppink', 'deepskyblue', 'dimgray', 'dimgrey', 'dodgerblue', 'firebrick', 'floralwhite', 'forestgreen', 'fuchsia', 'gainsboro', 'ghostwhite', 'gold', 'goldenrod', 'gray', 'green', 'greenyellow', 'grey', 'honeydew', 'hotpink', 'indianred', 'indigo', 'ivory', 'khaki', 'lavender', 'lavenderblush', 'lawngreen', 'lemonchiffon', 'lightblue', 'lightcoral', 'lightcyan', 'lightgoldenrodyellow', 'lightgray', 'lightgreen', 'lightgrey', 'lightpink', 'lightsalmon', 'lightseagreen', 'lightskyblue', 'lightslategray', 'lightslategrey', 'lightsteelblue', 'lightyellow', 'lime', 'limegreen', 'linen', 'magenta', 'maroon', 'mediumaquamarine', 'mediumblue', 'mediumorchid', 'mediumpurple', 'mediumseagreen', 'mediumslateblue', 'mediumspringgreen', 'mediumturquoise', 'mediumvioletred', 'midnightblue', 'mintcream', 'mistyrose', 'moccasin', 'navajowhite', 'navy', 'oldlace', 'olive', 'olivedrab', 'orange', 'orangered', 'orchid', 'palegoldenrod', 'palegreen', 'paleturquoise', 'palevioletred', 'papayawhip', 'peachpuff', 'peru', 'pink', 'plum', 'powderblue', 'purple', 'rebeccapurple', 'red', 'rosybrown', 'royalblue', 'saddlebrown', 'salmon', 'sandybrown', 'seagreen', 'seashell', 'sienna', 'silver', 'skyblue', 'slateblue', 'slategray', 'slategrey', 'snow', 'springgreen', 'steelblue', 'tan', 'teal', 'thistle', 'tomato', 'turquoise', 'violet', 'wheat', 'white', 'whitesmoke', 'yellow', 'yellowgreen']

row_colours = ['aqua','bisque','deepskyblue','hotpink','chartreuse','cornflowerblue','crimson','gold','darkorange','orchid','hotpink','fuchsia']
col_colours = ['lightpink','aquamarine','moccasin','turquoise','lime','khaki','greenyellow','yellow','pink','peachpuff','orange',
               'skyblue','white','goldenrod','lavenderblush','lavender','lightyellow','lightsalmon']

rgb_type = ["modifier","underglow","keylight","indicator","n/a","n/a","n/a","n/a","none","all"]
rgb_colours = ["lime","aqua","gold","crimson","lightsalmon","lavender","orange","pink","peachpuff","moccasin"]
swx = 19.05
swy = 19.05

cox = -7.5
coy = -7.0
rox = -7.5
roy = -7.0

rad = 2.5
radius = 1.0

def flag2x(c):
    if c not in [0,255]:
       x = int(log(c,2))
    elif c == 0:
       x = 8
    else:
       x = 9
    return x

def draw_kb(canvas_width=600, canvas_height=300,keys=None, encoders=None, centers=None, labels=None, iso=None, bae=None, rows=None,
            cols=None, output_path=None, svg=None, tooltips=None, rgbs=None, backlights=None):

    d = draw.Drawing(canvas_width+5, canvas_height+5, origin=(-5,-5))
    d.set_pixel_scale(5)
    r=draw.Rectangle(-5,-5,canvas_width+5,canvas_height+5,fill="black",rx=rad,ry=rad)
    d.append(r)
    sw = 0.5
    fs=6
    if keys:
        for ir in keys:
            d.append(draw.Rectangle(*ir, rx=rad,ry=rad,fill='darkslategray',stroke='deeppink',stroke_width=sw))
    if encoders:
        arrowcw = draw.Marker(-0.1, -0.51, 0.9, 0.5, scale=4, orient='auto')
        arrowcw.append(draw.Lines(-0.1, 0.5, -0.1, -0.5, 0.9, 0, fill='orchid', close=True))

        arrowccw = draw.Marker(-0.1, -0.51, 0.9, 0.5, scale=4, orient='auto')
        arrowccw.append(draw.Lines(-0.1, 0.5, -0.1, -0.5, 0.9, 0, fill='teal', close=True))
        for enc in encoders:
            d.append(draw.Circle(enc[0],enc[1],r=enc[2],stroke='fuchsia',stroke_width=sw))
            da = 0.0005*math.pi/180.0
            a = 45 * math.pi/180.0
            d.append(draw.Line(enc[0]+enc[2]*math.cos(a), enc[1]+enc[1]*math.sin(a),
                               enc[0]+enc[2]*math.cos(a+da),
                               enc[1]+math.sin(da+a)*enc[1],stroke='fuchsia', stroke_width=sw, fill='none',marker_end=arrowcw))
            a = 225 * math.pi/180.0
            d.append(draw.Line(enc[0]+enc[2]*math.cos(a), enc[1]+enc[1]*math.sin(a),
                               enc[0]+enc[2]*math.cos(a+da),
                               enc[1]+math.sin(da+a)*enc[1],stroke='fuchsia', stroke_width=sw, fill='none',marker_end=arrowcw))
            a = 135 * math.pi/180.0
            d.append(draw.Line(enc[0]+enc[2]*math.cos(a), enc[1]+enc[1]*math.sin(a),
                               enc[0]+enc[2]*math.cos(a-da),
                               enc[1]+math.sin(-da+a)*enc[1],stroke='fuchsia', stroke_width=sw, fill='none',marker_end=arrowccw))
            a = 315 * math.pi/180.0
            d.append(draw.Line(enc[0]+enc[2]*math.cos(a), enc[1]+enc[1]*math.sin(a),
                               enc[0]+enc[2]*math.cos(a-da),
                               enc[1]+math.sin(a-da)*enc[1],stroke='fuchsia', stroke_width=sw, fill='none',marker_end=arrowccw))

    if centers and rows:
        for c in centers:
            d.append(draw.Circle(c[0]+cox,c[1]+coy,r=radius,fill='fuchsia',stroke_width=sw))

    if centers and cols:
        for c in centers:
            d.append(draw.Circle(c[0]+rox,c[1]+roy,r=radius,fill='limegreen',stroke_width=sw))
    if rows:
        for ir in rows:
            z = [(rows[ir][i],rows[ir][i+1]) for i in range(0,len(rows[ir]),2)]
            z = sorted(z,key=lambda y: y[1],reverse=False)
            z = sorted(z,key=lambda x: x[0])
            w = list(sum(z, ()))
            e = draw.Lines(*w,stroke=row_colours[ir%len(row_colours)],close=False,fill='none',stroke_width=sw)
            e.append_title(f"row {ir}")
            d.append(e)
    if cols:
        for c in cols:
            try:
                ic = c[0]
            except:
                ic = c
            e = draw.Lines(*cols[c],stroke=col_colours[ic%len(col_colours)],close=False,fill='none',stroke_width=sw)
            e.append_title(f"column {c}")
            d.append(e)
    if iso:
        for e in iso:
            for i in range(6):
                d.append(draw.Line(*e[2*i],stroke='deeppink',fill='none',stroke_width=sw))
                d.append(draw.ArcLine(*e[2*i+1],stroke='deeppink',fill='none',stroke_width=sw))
    if bae:
        for e in bae:
            for i in range(6):
                d.append(draw.Line(*e[2*i],stroke='deeppink',fill='none',stroke_width=sw))
                d.append(draw.ArcLine(*e[2*i+1],stroke='deeppink',fill='none',stroke_width=sw))
    if labels and centers:
        for c,l in zip(centers,labels):
            d.append(draw.Text(l,fs,c[0],c[1],fill='peachpuff',center=True))

    if tooltips and centers:
        for c,t in zip(centers,tooltips):
            e = draw.Circle(c[0],c[1],r=5,fill='lightpink',stroke='none',opacity='0.16' ,stroke_width=sw)
            e.append_title(t)
            d.append(e)
    if rgbs:
        for c in rgbs:
            x = flag2x(c[2])
            e = draw.Circle(c[0],c[1],r=radius,fill=rgb_colours[x],stroke_width=sw)
            e.append_title("rgb role: "+rgb_type[x])
            d.append(e)
    if backlights:
        for c in backlights:
            x = flag2x(c[2])
            e = draw.Circle(c[0],c[1],r=radius*2,fill="orangered",stroke_width=sw)
            e.append_title("underglow flag: "+rgb_type[x])
            d.append(e)

    if svg:
      save_svg(d,output_path,svg)

def isoenter_key(x,y,w=1.25,h=2,swx=19.05,swy=19.05,rad=2.5):
    return [(x*swx-0.25*swx+rad,y*swy,x*swx+w*swx-rad,y*swy),
           (x*swx+w*swx-rad,y*swy+rad, rad, 0, 90),
           (x*swx+w*swx,y*swy+rad,x*swx+w*swx,y*swy+h*swy-rad),
           (x*swx+w*swx-rad,y*swy+h*swy-rad, rad, -90, 0),
           (x*swx+w*swx-rad,y*swy+h*swy,x*swx+rad,y*swy+h*swy),
           (x*swx+rad,y*swy+h*swy-rad, rad, 180, 270),
           (x*swx,y*swy+h*swy-rad,x*swx,y*swy+h*swy-1.0*swy+rad),
           (x*swx-rad,y*swy+h*swy-1.0*swy+rad, rad, 0, 90),
           (x*swx-rad,y*swy+h*swy-1.0*swy,x*swx-0.25*swx+rad,y*swy+h*swy-1.0*swy),
           (x*swx-0.25*swx+rad,y*swy+h*swy-1.0*swy-rad, rad, 180, 270),
           (x*swx-0.25*swx,y*swy+h*swy-1.0*swy-rad,x*swx-0.25*swx,y*swy+rad),
           (x*swx-0.25*swx+rad,y*swy+rad, rad, 90,180)]

def baeenter_key(x,y,w=1.5,h=2,swx=19.05,swy=19.05,rad=2.5):
    return [(x*swx+rad,y*swy,x*swx+w*swx-rad,y*swy),
            (x*swx+w*swx-rad,y*swy+rad, rad, 0, 90),
            (x*swx+w*swx,y*swy+rad,x*swx+w*swx,y*swy+h*swy-rad),
            (x*swx+w*swx-rad,y*swy+h*swy-rad, rad, -90, 0),
            (x*swx+w*swx-rad,y*swy+h*swy,x*swx+w*swx-2.25*swx+rad,y*swy+h*swy),
            (x*swx+w*swx-2.25*swx+rad,y*swy+h*swy-rad, rad, 180, 270),
            (x*swx+w*swx-2.25*swx,y*swy+h*swy-rad,x*swx+w*swx-2.25*swx,y*swy+h*swy-1.0*swy+rad),
            (x*swx+w*swx-2.25*swx+rad,y*swy+h*swy-1.0*swy+rad, rad, 90, 180),
            (x*swx+w*swx-2.25*swx+rad,y*swy+h*swy-1.0*swy,x*swx-rad,y*swy+h*swy-1.0*swy),
            (x*swx-rad,y*swy+h*swy-1.0*swy-rad, rad, -90, 0),
            (x*swx,y*swy+h*swy-1.0*swy-rad,x*swx,y*swy+rad),
            (x*swx+rad,y*swy+rad, rad, 90,180)]

def save_svg(d,out_path,file_name):
    f = file_name
    if out_path:
        f= Path(out_path,f)
    else:
        out_path='.'
    d.save_svg(f)

    cli.log.info('Wrote out {fg_cyan}%s/%s', out_path,file_name)
