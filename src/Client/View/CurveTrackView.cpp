#include "CurveTrackView.h"

CurveTrackView::CurveTrackView(const SdlTexture& tex, const int& angle) :
	ObjectView(tex), angle(angle) {
	Area srcArea(0, 0, 691, 691);
	clips.push_back(srcArea);
	horizontalScale = HOR_PS_CURVE_TRACK;
	verticalScale = VER_PS_CURVE_TRACK;
}

void CurveTrackView::drawAt(int x, int y) {
	Area srcArea = clips.front();
	Area destArea(initialX + x - horizontalScale/2,
				  initialY + y - verticalScale/2,
				  horizontalScale, verticalScale);
	texture.render(srcArea, destArea, (double) angle, SDL_FLIP_NONE);
}

int CurveTrackView::getAngle() {
	return angle;
}
