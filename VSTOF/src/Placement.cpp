#include "Placement.h"

Parameteres::Parameteres() { //全パラメーター分のフレーム作成
				//フレーム生成
				//frames.add(frame *parent, frame *self, std::string name, int length, bool lock)
	frames.add(nullptr, &p_frame.root, "root", 0, 0);
	frames.add(&p_frame.root, &p_frame.all, "all", 0, 0);
	frames.add(&p_frame.root, &p_frame.scroll, "scroll", 16, 1);
	frames.add(&p_frame.all, &p_frame.tone, "tone", 0, 0); //音色設定フレーム生成
	frames.add(&p_frame.tone, &p_frame.make_auto, "make_auto", 200, 0); //自動で音色を生成するか
	frames.add(&p_frame.tone, &p_frame.raw_wave_para, "raw_wave_para", 0, 0); //下記インデントを束ねる
	frames.add(&p_frame.raw_wave_para, &p_frame.use_rawwave, "use_rawwave", 100, 0); //生波形データの使用をするかどうか
	frames.add(&p_frame.raw_wave_para, &p_frame.rawwave, "rawwave", 100, 0); //生波形の使用部分波形(ファイルマッピングにするかも(´・ω・｀))
	frames.add(&p_frame.raw_wave_para, &p_frame.base_pitch, "base_pitch", 100, 0); //基音位置(手動変更可)
	frames.add(&p_frame.tone, &p_frame.tone_para, "tone_para", 0, 0); //下記インデントのフレームを束ねる
	frames.add(&p_frame.tone_para, &p_frame.change_natural, "change_natural", 100, 0); //音程変化を自然にするか
	frames.add(&p_frame.tone_para, &p_frame.overtones, "overtones", 100, 0); //倍音グラフor共鳴スペクトルグラフ
	frames.add(&p_frame.tone_para, &p_frame.iovertones, "iovertones", 100, 0); //倍音の数
	frames.add(&p_frame.tone_para, &p_frame.wave_type, "wave_type", 100, 0); //基本波形の形
	frames.add(&p_frame.tone_para, &p_frame.hostpar, "hostpar", 0, 0); //下記インデントのフレームを束ねる
	frames.add(&p_frame.hostpar, &p_frame.vol, "vol", 100, 0); //音量
	frames.add(&p_frame.hostpar, &p_frame.pitch, "pitch", 100, 0); //音程(IDI値に加算)
	frames.add(&p_frame.tone, &p_frame.wave_limit, "wave_limit", 100, 0); //波形の絶対値の上限
	frames.add(&p_frame.tone, &p_frame.outwave, "outwave", 100, 0); //出力波形
	frames.add(&p_frame.all, &p_frame.fade, "fade", 0, 0); //フェード設定フレーム生成
	frames.add(&p_frame.fade, &p_frame.use_string_mode, "use_string_mode", 100, 0); //弦モードの使用をするかどうか
	frames.add(&p_frame.fade, &p_frame.use_fade_change, "use_fade_change", 100, 0); //なめらかな音程,音量の変化を使用するかどうか(2つ同時に音を出せない)
	frames.add(&p_frame.fade, &p_frame.fadein, "fadein", 0, 0);
	frames.add(&p_frame.fadein, &p_frame.fadein_vol, "fadein_vol", 100, 0);
	frames.add(&p_frame.fadein, &p_frame.fadein_pitch, "fadein_pitch", 100, 0);
	frames.add(&p_frame.fade, &p_frame.fadeout, "fadeout", 0, 0);
	frames.add(&p_frame.fadeout, &p_frame.fadeout_vol, "fadeout_vol", 100, 0);
	frames.add(&p_frame.fadeout, &p_frame.fadeout_pitch, "fadeout_pitch", 100, 0);
	frames.add(&p_frame.fade, &p_frame.fadechange, "fadechange", 0, 0);
	frames.add(&p_frame.fadechange, &p_frame.fadechange_vol, "fadechange_vol", 100, 0);
	frames.add(&p_frame.fadechange, &p_frame.fadechange_pitch, "fadechange_pitch", 100, 0);

	//set_parent(frame *self, bool mode, int gap)
	frames.set_parent(&p_frame.root, 1, 2);
	frames.set_parent(&p_frame.hostpar, 1, 2);
	frames.set_parent(&p_frame.fadein, 1, 2);
	frames.set_parent(&p_frame.fadeout, 1, 2);
	frames.set_parent(&p_frame.fadechange, 1, 2);
	frames.set_parent(&p_frame.fadechange, 1, 2);

	frames.get_length(&p_frame.root); //全フレームのlength等取得
}

Parameteres::~Parameteres() {
	frames.all_dispose(&p_frame.root);
}

bool Draw::resize(WINDOW_INFO win_info) { //描画領域変更
									//win_infoのウィンドウサイズ更新
	GetClientRect(win_info.hwnd, &win_info.window);
	win_info.size.x = win_info.window.right - win_info.window.left;
	win_info.size.y = win_info.window.bottom - win_info.window.top;
	para.p_frame.root.pos.left = 0;
	para.p_frame.root.pos.top = 0;
	para.p_frame.root.pos.right = win_info.size.x;
	para.p_frame.root.pos.bottom = win_info.size.y;
	fps = win_info.fps;
	para.frames.resize(&para.p_frame.root, para.p_frame.root.pos); //全フレームの自動配置
	para.p_frame.all.pos.bottom = para.p_frame.all.length;
	para.frames.resize(&para.p_frame.all, para.p_frame.all.pos);
	gui.set_fps(fps);
	return 0;
}

void Draw::loop() { //ループ中に呼び出す関数
			  //画面初期化
	gui.reset();
	//フレームの境界線描画
	gui.FrameLine(&para.p_frame.root);
	//フレームの名称描画
	gui.FrameName(&para.p_frame.root);
	//各パラメーター描画
	{
		gui.wave_gui(&para.p_frame.make_auto, para.p_value->outwave, para.p_value->noutwave, 0);
		gui.sw(&para.p_frame.raw_wave_para, &a);
		gui.volume_gui(&para.p_frame.rawwave, &b);
	}
	//毎フレーム呼び出し関数
	gui.loop();
}