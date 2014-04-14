/* �����R�[�h�͂r�i�h�r ���s�R�[�h�͂b�q�k�e */
/* $Id$ */

#pragma once


#define UTVIDEO_VERSION_MAJOR  13
#define UTVIDEO_VERSION_MINOR   3
#define UTVIDEO_VERSION_BUGFIX  1

#define UTVIDEO_VERSION_MASK 0xffffff00
#define UTVIDEO_VERSION ((UTVIDEO_VERSION_MAJOR << 24) | \
                         (UTVIDEO_VERSION_MINOR << 16) | \
                         (UTVIDEO_VERSION_BUGFIX << 8))

#define UTVIDEO_GEN_VERSION0(a, b, c) (#a "." #b "." #c)
#define UTVIDEO_GEN_VERSION(a, b, c) UTVIDEO_GEN_VERSION0(a, b, c)
#define UTVIDEO_VERSION_STR UTVIDEO_GEN_VERSION(UTVIDEO_VERSION_MAJOR, UTVIDEO_VERSION_MINOR, UTVIDEO_VERSION_BUGFIX)
