Gallery Tilt Shift Plugin
=========================

What is it?
-----------

*Gallery Tilt Shift Plugin* is a filter for the Gallery application of the N9
(and N950) MeeGo devices which makes a picture look like a miniature.

From the [Wikipedia] [1],
> *Tilt-shift photography* refers to the use of camera movements on
> small- and medium-format cameras, and sometimes specifically refers
> to the use of tilt for selective focus, often for simulating a miniature
> scene. Sometimes the term is used when the shallow depth of field is simulated
> with digital post-processing; the name may derive from the tilt-shift lens
> normally required when the effect is produced optically.

Making an photograph of a life-size location or object look like photograph
of a miniature scale model is known as [Miniature faking] [2]. There are several
ways to achieve that result, and *fake Tilt Shift* is one of them.

Installing:
-----------

The easiet way of installing Gallery Tilt Shift Plugin is through its
entry in the [Nokia Store](http://store.ovi.com/content/274188).

Applying Tilt Shift to a picture:
---------------------------------
 * From the application grid, open Gallery
 * Pick a picture with a size equal or smaller than 512x512px (see Known Issues below)
 * Tap on the object menu and select *Edit*
 * Tap on *Tilt Shift*
 * Select the orientation of the focus (horizontal / vertical). In that
   orientation, the focused area will take all available space
 * Select the radius of the focus: that will rule the size of the focused
   area in the other orientation
 * Tap on the screen to center the focus around the given point

Usage explanation:
------------------

For a brief usage explanation visit Gallery Tilt Shift Plugin's
[page](http://igalia.github.com/gallery-tiltshift-plugin/) or this
[video explanation](http://youtu.be/fkTn1E1T9B8).

Known Issues:
-------------
 * Only *PR >= 1.2* is currently supported.
 * You will need to reboot to make the icon for the plugin appear properly
 * Due to limitations in the current implementation of the filter, operating on
   images larger than 512x512 is very slow and causes artifacts.

In depth:
---------
Gallery is using [MeeGo Image Editor] [3] to apply filters on images. Since it uses
a plugin mechanism, it is possible to write new filters and have them appearing
into the built-in Gallery.

MeeGo Image Editor uses tiling to operate on small portions of an image and
avoid having to load it all into memory. While this approach works pretty well
for many filters, it fails for those requiring context to be applied.

In the case of the Tilt Shift plugin, this is important because it has been
implemented by:

 1. Blurring the image ussing a Gaussian Blur filter
 1. Keep an area of the image focused (either vertically or horizontally)
 1. Combine both parts of the image using a Gaussian filter (so the focus is lost
    gradually from the focused area to the rest of the image)
 1. Increase the saturation, so the colors seems those of a miniature

However, the Gaussian Blur needs a context when it is applied to a pixel, so when
it reaches the borders of a tile, it will have to "guess" a pixel. The larger the
Gaussian Blur kernel, the bigger the amount of pixels it has to guess. This produces
very noticeable artifacts on the borders of the tiles. Besides, its performance
is not very good, so applying it to a full size image takes a lot of time to finish.

[1]: http://en.wikipedia.org/wiki/Tilt-shift_photography "Tilt-shift photography"
[2]: http://en.wikipedia.org/wiki/Miniature_faking "Miniature Faking"
[3]: https://maemo.gitorious.org/meego-image-editor/ "MeeGo Image Editor"
