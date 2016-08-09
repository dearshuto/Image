# 画像の入出力ライブラリ


## How To Compile
### コンパイルに必要なもの
* git
* cmake
* python

gitとcmakeはシステムのパスが通っている必要があります。  
pythonはgitとcmakeのコマンドをスクリプトかするのに使用しています。手動でgitとcmakeのコマンドを走らせればとくに必要ありません。

### pythonを使ったコンパイル環境構築
> setup.py を叩く。おしまい！

### gitとcmakeのコマンドを各自で叩く方法

> * git submodule update --init --recursive を実行して必要なファイルをダウンロードする
> * 外部ライブラリのセットアップ  
   * unix: external/image/external/jpeg の中のconfigureをたたく
   * Windows:  
   copy jconfig.vc jconfig.h を実行
> * cmake -DBUILD_STATIC=ON -DBUILD_SHARED_LIBS=OFF を開発環境に合わせて走らせる

ファイルを追加, 移動したさいには, cmake_generator.pyを叩くと開発環境が更新されます。
