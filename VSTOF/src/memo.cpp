//frames.add(frame *parent, frame *self, std::string name, int length, bool lock)
frames.add(nullptr, &p_frame.root, "root", 0, 0);
frames.add(&p_frame.root, &p_frame.all, "all", 0, 0);
frames.add(&p_frame.root, &p_frame.scroll, "scroll", 16, 1);
frames.add(&p_frame.all, &p_frame.tone, "tone", 0, 0); //音色設定フレーム生成
frames.add(&p_frame.tone, &p_frame.make_auto, "make_auto", 100, 0); //自動で音色を生成するか
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

//UIのアニメーション効果管理クラスの設計メモ
/*
大体の流れメモ
アニメーションクラスにフレーム変数配列を
vectorで作り、それを各フレームの保持情報に
追加する。宣言時はどのフレームの
アニメーション変数配列の配列長は0
各UI描画関数がそれを参照し、配列の長さが
足りない場合、配列を拡張し、
決められた値に初期化
足りている場合は放置
アニメーション変数の中身は、
time{
double t; //fpsに依存しない時間軸変数(0.0<=t<=1.0)
char val; //変化グラフの種類
}
val = 0 -> 変化はそのまま
val = 1 -> y=x^2
val = 2 -> y=-(x-1)^2+1
val = 3 -> y=3*(x^2)-2*(x^3) <-三次関数のやつ

また、アニメーション時の変化グラフを各種
揃えた関数も用意
また、コンストラクタの引数などにfpsを引数に取り、
fpsを取得
*/

//GUIクラスの変更すべき点
/*
Drawクラスで各GUIパーツを配置し、
あとはGUIクラスのループ関数を
Drawクラスで毎フレーム呼び出すだけで
良いようにする。
そうすることにより、アニメーション変数などの
管理が簡単かつわかりやすくなる為。
*/

//塗りつぶしの仕様変更について
/*
f->size.x<num_sampleの時は塗りつぶし処理特殊
指定範囲内の多角形描画
A->B
 ↑
percent変換
```
float *samples; //波形配列
int height; // 描画高さ
for(int j = 0; j < width; j++){
	percent(
		i,
		0,
		width,
		(int)samples[index],
		(int)samples[index+1]
	);
}
```
*/