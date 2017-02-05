VSTOF
====

VSTiの描画部分

## 概要
VSTiプラグインの設定画面です。  
VSTiのプロセスとは別プロセスで起動します。  
VSTiがホストプロセスで、このプログラムとはメモリ共有で通信しています。  
描画にはOpenFrameworksを用いています。  
コンパイルするには、プロジェクトにOpenFrameworksを追加してください。  
アニメーションエフェクトを多く使用したいので、UIのパーツから作っています。  

## 進行状況
2017/01/29:  
最近ボタンができた(∩´∀｀)∩  
![ボタン](https://raw.githubusercontent.com/wakewakame/VSTOF/master/Demo/button.gif)  
  
2017/01/31:  
全然関係ないけど明日から5日間受験や  
でも多分更新する  
  
2017/02/01:  
波形描画関数できた(∩´∀｀)∩  
![グラフ](https://raw.githubusercontent.com/wakewakame/VSTOF/master/Demo/wave.gif)  
波形がたまにｶﾞｸｯってなるのはホストプログラムの仕様  
原因はわかってるから、こっちがある程度完成してから治す  