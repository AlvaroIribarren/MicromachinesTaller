#include "ExplosionView.h"
#include <iostream>

ExplosionView::ExplosionView(const SdlTexture& tex) : ObjectView(tex) {
	for (int i = 0; i < 5; ++i) {
		Area area(i*90, 0, 90, 90);
		clips.push_back(area);
	}
	for(unsigned i = 0; i < 5; ++i) {
		Area area(i*90, 90, 90, 90);
		clips.push_back(area);	
	}
	frame = 0;
	totalFrames = 0;
	done = false;
	horizontalScale = 100;
	verticalScale = 100;
}

void ExplosionView::drawAt(int x, int y) {
	if (!done) {
		Area srcArea = clips.at(floor(frame/4));
		Area destArea(initialX + x - horizontalScale/2,
					  initialY + y - verticalScale/2, 
					  horizontalScale, verticalScale);
		texture.render(srcArea, destArea);
		++frame;
		++totalFrames;
		if ((frame/4) >= 10) {
			frame = 0;
		}
		if ((totalFrames/4) >= 20) {
			done = true;
		}
	}
}