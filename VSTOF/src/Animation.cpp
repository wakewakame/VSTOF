#include "Animation.h"

void Animation::set_fps(double frame_rate) {
	fps = frame_rate;
}

int Animation::get_len() {
	return t.size();
}

void Animation::add(double length, char effect_type, bool *sw) {
	motion.push_back(0);
	len.push_back(length);
	t.push_back(0.0);
	m.push_back(0.0);
	p.push_back(0.0);
	type.push_back(effect_type);
	swp.push_back(sw);
	bsw.push_back(*sw);
}

void Animation::loop() {
	for (int i = 0; i < t.size(); i++) {
		//前フレームとの値が違う場合、アニメーション開始
		if (*swp[i] != bsw[i]) {
			motion[i] = 1;
			bsw[i] = *swp[i];
		}
		//アニメーション処理
		if ((motion[i]) || (type[i] == 4)) {
			//経過時間計算
			if (*swp[i]) {
				t[i] += (1.0 / fps);
				if (t[i] >= len[i]) {
					t[i] = len[i];
					motion[i] = 0;
				}
			}
			else {
				t[i] -= (1.0 / fps);
				if (t[i] <= 0.0) {
					t[i] = 0.0;
					motion[i] = 0;
				}
			}
			//モーション計算
			p[i] = t[i] / len[i];
			switch (type[i]) {
			case 0:
				//y=x
				m[i] = p[i];
				break;
			case 1:
				//y=x^2
				m[i] = pow(p[i], 2);
				break;
			case 2:
				//y=-(x-1)^2+1
				m[i] = 1.0 - pow(p[i] - 1.0, 2);
				break;
			case 3:
				//y=3*(x^2)-2*(x^3)
				//x=0でyが極小値0となり、x=1でyが極大値1となる三次関数
				m[i] = 3 * pow(p[i], 2) - 2 * pow(p[i], 3);
				break;
			case 4:
				//指定時間を0.0-1.0区間ループ
				if (motion[i] == 0) {
					motion[i] = 1;
					t[i] = 0.0;
				}
				m[i] = p[i];
				break;
			}
		}
	}
}