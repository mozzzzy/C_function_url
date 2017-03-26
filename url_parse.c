/*
 *usage 
 * url_parse()関数にchar url[] もしくは char *url を渡すとrequest_info構造体に
 * 情報を入れて返す。
 *
 *test main関数
 * cc -c -DDEBUG ~~~.c
 * cc -o ~~~.exe ~~~.o
 */


#include<stdio.h>
#include<string.h>

#define BUF_LEN 512

struct request_info{
        char filepath[BUF_LEN];		// 要求するファイルパス
        unsigned short port;		// 接続するポート番号
        char fqdn[BUF_LEN];		// 接続するFQDN
        char fqdn_no_port[BUF_LEN];		// 接続するFQDN , ポートなし
        char filename[BUF_LEN];		// 要求するファイル名
        char fqdn_and_filepath[BUF_LEN];	// 接続するFQDN + パス
};

struct request_info url_parse(char *url){
	struct request_info request_one;	//本関数がmain関数に返すための構造体

	/*urlを編集するため別アドレスにコピー*/
	char url_copy[BUF_LEN];
	strcpy(url_copy, url);

	/*urlからfqdn+filepathを抽出*/
	char *protocol_checker;		//http://があるかどうかの確認用
	char fqdn_and_filepath[BUF_LEN];	//プロトコルをカットした文字列
	if ((protocol_checker = strstr(url_copy,"http://"))!=NULL){
		strcpy(request_one.fqdn_and_filepath, protocol_checker+7);	
	}else{
		strcpy(request_one.fqdn_and_filepath, url_copy);
	}


	/*fqdn_and_filepathから fqdn と ファイルパス を抽出*/
	char* srash_checker;
	char fqdn_and_filepath_copy[BUF_LEN];	//fqdn_and_filepathを編集するために別アドレスへコピー
	strcpy(fqdn_and_filepath_copy, request_one.fqdn_and_filepath);
	if((srash_checker = strchr(fqdn_and_filepath_copy, '/'))!=NULL){	//ホスト名の後ろにファイルパスがあった場合 ('/'が存在した場合)
		*srash_checker = '\0';
		/*fqdnの抽出*/
		strcpy(request_one.fqdn, fqdn_and_filepath_copy);
		/*ファイルパスの抽出*/
		strcpy(request_one.filepath, "/");	//はじめに'/'を入れる
		strcat(request_one.filepath, srash_checker+1);

	}else{	//fqdnの後ろにファイルパスがなかった場合 ('/'が存在しなかった場合)
		/*fqdnの抽出*/
		strcpy(request_one.fqdn, fqdn_and_filepath_copy);
		/*ファイルパスの抽出*/
		strcpy(request_one.filepath, "/");	//ファイルパスが存在しなかったら'/'を入れる
	}


	/*fqdn から portを抽出*/
	char fqdn_copy[BUF_LEN];
	char* colon_checker;	//':'検索用
	char* port_char;
	strcpy(fqdn_copy, request_one.fqdn);
	if((colon_checker = strchr(fqdn_copy,':'))!=NULL){
		*colon_checker = '\0';
		strcpy(request_one.fqdn_no_port,fqdn_copy);
		request_one.port = atoi(colon_checker+1);
	}else{
		strcpy(request_one.fqdn_no_port,fqdn_copy);
		request_one.port = 80;
	}

	/*filepathからfilenameを抽出*/
	srash_checker = request_one.filepath;
	while((srash_checker = strchr(srash_checker,'/'))!=NULL){
		srash_checker = srash_checker + 1;
		strcpy(request_one.filename,srash_checker);
	}

	return request_one;
}

#ifdef DEBUG
int main(void){

	char url[] = "http://example.example.example.com:12345/d1/d2/d3/content.html";
	struct request_info request_one;
	request_one = url_parse(url);	
	printf("######request_one.fqdn_and_filepath = %s\n", request_one.fqdn_and_filepath);
	printf("######request_one.fqdn = %s\n", request_one.fqdn);
	printf("######request_one.fqdn_no_port = %s\n", request_one.fqdn_no_port);
	printf("######request_one.port = %d\n", request_one.port);
	printf("######request_one.filepath = %s\n", request_one.filepath);
	printf("######request_one.filename = %s\n", request_one.filename);


}
#endif
