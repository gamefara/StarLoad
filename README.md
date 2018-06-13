# ゲームについて  
Windowsの簡単なゲームです。  
事前にZIPファイルを解凍するソフト等を用意する必要があります。  
ファイル一覧の右上にある緑色のClone or downloadボタンをクリックし、  
Download ZIPをクリックしてダウンロードしてください。  
  
ZIPファイルを解凍し、PC上の任意の場所にフォルダに配置してください。
その後、StarLoad.exeファイルを実行すると遊ぶことができます。  
  
操作説明等はゲーム内のタイトル画面から閲覧することができます。  
ゲーム自体は1プレイ1,2分程度のものです。

___

# VisualStudioで実行する場合  
本作品はDXライブラリを使用しています。  
__VisualStudioで実行するためには、初めに設定することがあります。 __ 
  
VisualStudioのバージョンごとに少し異なりますが、  
http://dxlib.o.oo7.jp/dxuse.html  
上記のDXライブラリの使い方のページにバージョンごとの設定方法が記載されています。

閲覧箇所は、各バージョンごとに記載されている、  
「 __4.ＤＸライブラリを使用するためのプロジェクトの設定を行う__ 」です。  
フォルダパスを指定する箇所がありますが、  
__(プロジェクトフォルダ名)/DxLib_VC/プロジェクトに追加すべきファイル_VC用__ を指定してください。

___

# 備考
・exeファイルについて  
ディレクトリ場所を変えてしまうと、Resourceフォルダ内の素材等を読み込めなくなってしまうので、  
__exeファイルはプロジェクトフォルダ直下のまま__ にしてください。  

・Jsonファイルについて  
(プロジェクトフォルダ名)/Resource/SaveData  
上記ディレクトリにConfig.json,Result.jsonの2ファイルを配置しています。  
これらのファイルにデータを保存していますが、  
 - ファイルが存在しない  
 - ファイルの中身が破損している  
このような場合、 __デフォルト設定のファイルを新規作成するようにしています。__  

___

copyright (c) 2018 H.Okuyama.
