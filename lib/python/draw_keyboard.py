"""Draw an svg or render png for layout or matrix
"""
import drawsvg as draw
from milc import cli
from pathlib import Path

swx=19.025
swy=19.025

cox=-7.5
coy=-6.0
rox=-7.5
roy=-7.5

rad=2.5
radius = 1.0

def draw_kb(canvas_width=600, canvas_height=300,keys=None, centers=None, labels=None, iso=None, bae=None, rows=None,
            cols=None, output_path=None, svg=None, tooltips=None ):

    d = draw.Drawing(canvas_width+5, canvas_height+5, origin=(-5,-5))
    d.set_pixel_scale(5)
    r=draw.Rectangle(-5,-5,canvas_width+5,canvas_height+5,fill="midnightblue",rx=rad,ry=rad)
    d.append(r)
    sw = 0.5
    fs=6
    if keys:
        for ir in keys:
            d.append(draw.Rectangle(*ir, rx=rad,ry=rad,fill='none',stroke='deeppink',stroke_width=sw))
    if centers and rows:
        for c in centers:
            d.append(draw.Circle(c[0]+cox,c[1]+coy,r=radius,fill='mediumspringgreen',stroke_width=sw))

    if centers and cols:
        for c in centers:
            d.append(draw.Circle(c[0]+rox,c[1]+roy,r=radius,fill='orchid',stroke_width=sw))
    if rows:
        for ir in rows:
            e = draw.Lines(*rows[ir],stroke='orchid',close=False,fill='none',stroke_width=sw)
            e.append_title(f"row {ir}")
            d.append(e)
    if cols:
        for c in cols:
            e = draw.Lines(*cols[c],stroke='lightseagreen',close=False,fill='none',stroke_width=sw)
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
            d.append(draw.Text(l,fs,c[0],c[1],fill='mediumspringgreen',center=True))

    if tooltips and centers:
        for c,t in zip(centers,tooltips):
            e = draw.Circle(c[0],c[1],r=5,fill='chartreuse',stroke='none',opacity='0.16' ,stroke_width=sw)
            e.append_title(t)
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
