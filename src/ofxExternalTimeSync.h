#pragma once

#include "ofMain.h"

template <typename T>
class ofxExternalTimeSync : public T {
public:
	ofxExternalTimeSync()
		: skip_frame_threshold_in_sec(0.3)
		, precision(1. / 30)
		, freewheel_time(5)
		, time_diff(0)
		, target_time(0)
		, is_jam_syncing(false)
		, has_new_time(false)
		, sync_enable(true) {}

	void setTime(float playhead_in_sec) {
		// play if not playing
		if (!this->isPlaying()) {
			float pct = playhead_in_sec / this->getDuration();
			this->setPosition(pct);
			this->play();
		}

		target_time = playhead_in_sec;
		last_sync_update_time = ofGetElapsedTimef();

		has_new_time = true;
		is_jam_syncing = false;
	}

	void update() {
		if (!sync_enable) {
			T::update();
			return;
		}

		float t = ofGetElapsedTimef();
		float jam_time = t - last_sync_update_time;

		T::update();

		time_diff = target_time - (this->getPosition() * this->getDuration());

		if (freewheel_time > 0 && jam_time > freewheel_time) {
			this->stop();
			return;
		}

		if (jam_time > precision * 2) {
			is_jam_syncing = true;
			return;
		}

		if (!has_new_time) return;
		has_new_time = false;

		if (!this->isFrameNew()) return;

		float framedelta = time_diff / precision;

		if (fabs(time_diff) > skip_frame_threshold_in_sec) {
			float pct = target_time / this->getDuration();
			this->setPosition(pct);
			return;
		}

		if (fabs(time_diff) > precision) {
			float speed = 1 + ofClamp((framedelta * 0.05), -0.1, 0.1);
			this->setSpeed(speed);
		} else {
			this->setSpeed(1);
		}
	}

	inline void setSyncEnable(bool yn) { sync_enable = yn; }
	inline bool getSyncEnable() const { return sync_enable; }
	
	inline float getTimeDiff() const { return time_diff; }

	inline bool isSyncing() const { return sync_enable && fabs(time_diff) < precision; }
	inline bool isJamSyncing() const { return is_jam_syncing; }

	inline void setFreeWhellTime(float v) { freewheel_time = v; }
	inline float getFreeWheelTime() const { return freewheel_time; }
	
protected:
	float precision;
	float time_diff;

	float skip_frame_threshold_in_sec;
	float target_time;

	float last_sync_update_time;
	float freewheel_time;

	bool is_jam_syncing;
	bool has_new_time;

	bool sync_enable;
};
