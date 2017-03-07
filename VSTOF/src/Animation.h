//アニメーション変数管理クラス
class Animation {
private:
	//変数宣言
	double fps; //フレームレート
	std::vector<bool> motion; //アニメーションが開始されていれば1,それ意外は0
	std::vector<double> len; //アニメーションする時間(単位はsec)
	std::vector<double> t; //fpsに依存しない経過時間(0.0<=t<=len)
	std::vector<char> type; //アニメーションの変化の種類(0<=val)
	std::vector<bool*> swp; //sw=0ならtを0に近づけ、逆なら逆
	std::vector<bool> bsw; //前フレームのときのsw1

public:
	//変数宣言
	std::vector<double> m; //モーション座標(基本的に 0.0<=m<=1.0)
	std::vector<double> p; //(p/1.0)=(t/len)

	//関数宣言
	//fps指定
	void set_fps(double frame_rate);
	//アニメーション配列の長さ
	int get_len();
	//アニメーション変数の追加
	void add(double length, char effect_type, bool *sw);
	//毎フレーム呼び出し関数
	void loop();
};