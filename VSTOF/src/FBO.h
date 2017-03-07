//フレームバッファクラス
class FBO {
public:
	/*
	///メモ///
	アルファフレームバッファは必要なときだけ生成する
	*/
	//フレームバッファクラスの配列
	std::vector<ofFbo> color;
	std::vector<ofFbo> alpha;
	//現在のフレームバッファの個数
	int num;
	//現在使用しているフレームバッファのインデックス(-1ならレンダーバッファ)
	int now_index;
	//仮フレームバッファクラスのコンストラクト
	ofFbo fbo_;
	//コンストラクタ
	FBO();
	//デストラクタ
	~FBO();
	//フレームバッファの追加
	int add(int x, int y);
	//カラーフレームバッファの切り替え(index=-1ならレンダーバッファ)
	void change_c(int index);
	//アルファフレームバッファの切り替え(index=-1ならレンダーバッファ)
	void change_a(int index);
	//指定カラーバッファ描画
	void draw_c(int x, int y, int index);
	//フレームサイズ取得関数
	POINT get_size(int index);
};