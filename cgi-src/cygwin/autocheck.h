#ifndef __AUTO_CHECK__H__
#define __AUTO_CHECK__H__

/* QUERY_STRING からハッシュを作って、初期設定をする */
extern void get_query();

extern void set_hash();

/* 作り方を選択する */
extern void set_complex();
extern void set_simple();

/* 地味さ加減を設定する */
extern void set_mild(int g, int nGray, int d, int nDark, int l, int nLight);

/* 今までの設定から一つずつ色を取得する */
extern void get_colors(unsigned char data[4*64]);

/* 画像データを作って返します */
extern void response(unsigned char data[4*64]);

#endif /* #ifndef __AUTO_CHECK__H__ */

