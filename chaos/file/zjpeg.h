/*******************************************************************************
**                                  LibChaos                                  **
**                                   zjpeg.h                                  **
**                          See COPYRIGHT and LICENSE                         **
*******************************************************************************/
#ifndef ZJPEG_H
#define ZJPEG_H

#include "zimage.h"
#include "yimagebackend.h"
#include "zbinary.h"
#include "zpath.h"

namespace LibChaos {

class ZJPEG : public YImageBackend {
public:
    struct JPEGError {
        enum jpegerrors {
            none = 0,
            badreadfile = 1,
            badwritefile = 2
        };
    };
    struct JPEGWrite {
        enum jpegoptions {
            none = 0,
        };
    };

public:
    ZJPEG(ZImage *image) : _image(image){}

    bool decode(ZBinary &jpegdata_in, ReadOptions *options = nullptr);
    bool encode(ZBinary &jepgdata_out, WriteOptions *options = nullptr);

    bool read(ZPath path);
    bool write(ZPath path, JPEGWrite::jpegoptions options = JPEGWrite::none);

private:
    ZImage *_image;
};

}

#endif // ZJPEG_H