# 画像の入出力ライブラリ

## How To Compile
> git submodule update --init --recursive を実行して必要なファイルをダウンロードする
> * unx: external/image/external/jpeg の中のconfigureをたたく
> * Windows: copy jconfig.vc jconfig.h を実行
> cmake -DBUILD_STATIC=ON -DBUILD_SHARED_LIBS=OFF を開発環境に合わせて走らせる
