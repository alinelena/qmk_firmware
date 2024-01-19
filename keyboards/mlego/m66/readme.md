# m66

![M65](https://mlego.elena.re/pics/m66/mlego_m66_rev4-lcd.webp)

A  (13x5) ortholinear keyboard that can be hand wired or using a pcb.

* Keyboard Maintainer: [Alin Elena](https://github.com/alinelena) ([@drFaustroll on GitLab](https://gitlab.com/drFaustroll))
* Hardware Supported: custom pcb [see](https://gitlab.com/m-lego/m65) with rp2040 on various boards, xiao seeduino rev1-rev3,
  promicro footprint rev4 (eg splinky)
* Hardware Availability: [custom how to](https://mlego.elena.re/m66)


Make example for this keyboard (after setting up your build environment):

For rev1

    qmk compile -kb  mlego/m66/rev1 -km default

Flashing example for this keyboard:

    qmk flash -kb  mlego/m66/rev2 -km default

To enter flashing mode, press Lower+Raise+b, or double click reset on the xiao board

For rev2

    qmk compile -kb  mlego/m66/rev2 -km default


Flashing example for this keyboard:

    qmk flash -kb  mlego/m66/rev2 -km default


To enter flashing mode, press Lower+Raise+b, or double click reset on the xiao board

For rev3

    qmk compile -kb  mlego/m66/rev3 -km default

Flashing example for this keyboard:

    qmk flash -kb  mlego/m66/rev3 -km default


For rev4

    qmk compile -kb  mlego/m66/rev4 -km default

Flashing example for this keyboard:


    qmk flash -kb  mlego/m66/rev4 -km default

To enter flashing mode, press Lower+Raise+b, or double click reset on the splinky board or on the pcb itself.


See the [build environment setup](https://docs.qmk.fm/#/getting_started_build_tools) and the [make instructions](https://docs.qmk.fm/#/getting_started_make_guide) for more information. Brand new to QMK? Start with our [Complete Newbs Guide](https://docs.qmk.fm/#/newbs).




