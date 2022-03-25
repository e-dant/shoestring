#! /bin/bash

function mkfavicon::main() {
    if [ $# != 1 ]; then
        return 1
    fi

    local sizes=("16" "32" "48" "64" "128" "256" "512")

    echo -n "=> "
    for i in ${sizes[@]}; do
        if [ -f "$i.png" ]; then
            echo "this would overwrite $i."
            echo "delete it explicitly before using this program."
            return 1
        fi
        echo -n "$i "
        convert 1024.png \
            -thumbnail "$ix$i>" \
            -gravity center \
            -background transparent \
            -extent "$i!x$i!" \
            "$i.png" || return 1
    done
    #echo

    #echo "=> ico"
    #convert 16.png 32.png 48.png 64.png 128.png 256.png 512.png favicon.ico

    #echo "[cleaning up]"
    #echo -n "rm "
    #for i in ${sizes[@]}; do
    #    echo -n "$i "
    #    rm "$i.png"
    #done
    #echo

    echo "done."
}

function mkfavicon::echo_usage () {
    echo "usage: mkfavicon [filename (should be greater than 512px)]"
}

if [ $# == 1 ] && [ -f "$1" ]; then
    mkfavicon_arg="$1"
elif [ $# == 0 ] && [ -f "1024.png" ]; then
    mkfavicon_arg="1024.png"
elif [ $# -gt 1 ]; then
    echo "strange arguments"
    mkfavicon::echo_usage
    exit 1
elif [ $# == 0 ] && [ ! -f "1024.png" ]; then
    echo "no filename provided and could not find 1024.png (the default)."
    mkfavicon::echo_usage
    exit 1
else
    mkfavicon::echo_usage
    exit 1
fi

mkfavicon::main $mkfavicon_arg

#
# https://www.digitalocean.com/community/tutorials/workflow-resizing-images-with-imagemagick
#
#Resize to specific dimensions, ignoring the aspect ratio
#
#Resizing to specific dimensions without preserving the aspect ratio
#tends to not be a desired thing to do. Images end up looking
#distorted, unless the original aspect ratio was already pretty close
#to 1:1.
#
#That said, it’s still good to know how to resize this way, if nothing
#else, so we know what to avoid when punching in commands late at
#night.
#
#To resize an image to specific dimensions, without maintaining the
#original image’s aspect ratio, include a bang ! at the end of the
#dimensions:
#
#$ convert original.png -resize 100x100! new.png Copy Resize to
#specific dimensions and keep aspect ratio
#
#To keep the aspect ratio, simply omit the bang:
#
#$ convert original.png -resize 100x100 new.png Copy This won’t
#actually resize the image to the exact dimensions specified. What it
#will do is resize the image to fit within those dimensions.
#
#The flip side, is that you can append ^ to the dimensions to tell
#ImageMagick that you’d like to resize the image to fill the
#dimensions, potentially overlapping on one side.
#
#One of the two dimensions (either width or height) will be scaled
#exactly, while the other will be scaled proportionately and may
#overlap:
#
#$ convert original.png -resize 100x100^ new.png Copy Resize to
#specific dimensions and keep the aspect ratio
#
#Sometimes you want to not only resize the image and fill the area, as
#well as keep it’s aspect ratio, but also crop it so there’s nothing
#overlapping. A good use case for this is user avatars. Obviously you
#don’t want to stretch or squash a user’s profile picture, so
#cropping it to a square is an okay solution:
#
#$ convert original.png -resize 100x100^ -gravity center -extent
#100x100 new.png Copy I say it’s only “okay” because users have grown
#more accustomed to being able to select the region of their uploaded
#image to serve as their avatar, so assuming they want the crop to
#the center of the image could cause a bit of a stir.
#
#Also, depending on your needs, you can adjust the gravity to grab
#align differently, or even crop a size that doesn’t marry up exactly
#with the scaled dimensions.
#