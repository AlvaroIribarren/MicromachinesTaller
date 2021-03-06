#include "StraightTrackView.h"
#include <iostream>

StraightTrackView::StraightTrackView(const SdlTexture& tex, const int& angle) :
	ObjectView(tex), angle(angle) {
	Area srcArea(0, 0, 688, 640);
	clips.push_back(srcArea);
	horizontalScale = HOR_PS_STRAIGHT_TRACK;
	verticalScale = VER_PS_STRAIGHT_TRACK;
}

void StraightTrackView::drawAt(int x, int y) {
	Area srcArea = clips.front();
	Area destArea(initialX + x - horizontalScale/2,
				  initialY + y - verticalScale/2, 
				  horizontalScale, verticalScale);
	texture.render(srcArea, destArea, (double) angle, SDL_FLIP_NONE);
}

int StraightTrackView::getAngle() {
	return angle;
}
