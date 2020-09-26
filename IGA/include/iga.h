#ifndef _IGA_H
#define _IGA_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>

#define CURL_STATICLIB

#include "curl/curl.h"

#include "json/json.h"

#include "sha1.h"

#define APR_DECLARE_STATIC

#include "apr/apr_general.h"
#include "apr/apr_pools.h"
#include "apr/apr_tables.h"
#include "apr/apr_strings.h"
#include "apr/apr_thread_proc.h"
#include "apr/apr_file_io.h"
#include "apr/apr_file_info.h"
#include "apr/apr_fnmatch.h"
#include "apr/apr_lib.h"

#define NUM_THREADS				64
#define MAX_TEXTURE				10

/*
#define IGA_INVENTORY_URL		"http://ad-worker.nexon.com/worker.php/frontend/getInventories?gameid=%s"
#define IGA_IMAGE_URL			"http://ad-worker.nexon.com/worker.php/frontend/getImages?gameid=%s"
#define IGA_SELECT_URL			"http://ad-worker.nexon.com/worker.php/frontend/select?gameid=%s&invid=%s&age=%d&gender=%d"
#define IGA_POST_URL			"http://ad-worker.nexon.com/worker.php/frontend/upload"
*/
#define IGA_INVENTORY_URL		"http://%s/igac/getInventories/%s"
#define IGA_IMAGE_URL			"http://%s/igac/getImages/%s?gameid=%s"
#define IGA_SELECT_URL			"http://%s/igac/select/%s/%s?gameid=%s&age=%d&gender=%d"
#define IGA_POST_URL			"http://%s/igac/upload/"
#define IGA_TEXTURE_URL			"http://%s/%s"

struct NIS_IGA {
	char			*gameid;
	char			*userid;
	int				age;
	int				gender;
	char			*host;
	char			*local_dir;
};

struct wd_in {
	int				start;
	int				size;
	char			*data;
};

int					curl_get(char *url, struct wd_in *wdi);
size_t				curl_write_data(void *buffer, size_t size, size_t nmemb, void *userp);
void				*APR_THREAD_FUNC curl_post(apr_thread_t *thd, void *data);


struct IGA_SELECT
{
	int				campaign_id;
	int				ads_id;
	int				creative_id;
	int				loc_id;
	char			*filepath;
	char			*filename;
	char			*sha1;
};

struct IGA_TEXTURE
{
	int				count;
	struct {
		char		*filename;
		char		*data;
		size_t		size;
	} texture[MAX_TEXTURE];
};

struct IGA_RECT {
    struct iga_point {
        double		x;
        double		y;
    } p[4];
};

int				iga_init(void);
int				iga_set_env(char *iga_host, char *gameid, char *local_dir);
int				iga_clear_images(void);
int				iga_set_user(char *userid, int age, char gender);
void			iga_clear(void);

int				iga_get_inventories(void);
char			*iga_fetch_inventory(int index);
int				iga_get_images(void);
char			*iga_fetch_image(int index);

int				iga_select(char *invid, struct IGA_SELECT* iga_select);

int				iga_start(void);
int				iga_track(char *invid, char *filename, double angle, struct IGA_RECT rect);
int				iga_refresh_pool(void);
int				iga_end(void);

int				iga_get_texture(char *invid, struct IGA_TEXTURE *t, int data_read, int sha1_compare);

apr_status_t	writeFile(apr_pool_t *pool, char *fname, char *buf, size_t size);
apr_status_t	readFile(apr_pool_t *pool, char *fname, char **buf, size_t *size);
int				compare_sha1(apr_pool_t *pool, char *localpath, char *remote_sha1);

#ifdef __cplusplus
}
#endif

#endif
