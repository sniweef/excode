#ifndef __HANDLE_DATA_H__
#define __HANDLE_DATA_H__

typedef struct deal_information {
	gint shop_id;						/*所属店铺编号*/
	gint deal_id;						/*交易编号*/
	gint pay_type;						/*支付类型*/
	gfloat deal_money;					/*交易金额*/
	gchar deal_date[20];				/*交易日期*/
	gchar customer_addr[20];			/*客户所在地*/
	struct deal_information *deal_next; /*指向下个节点*/
} DealInfo;
typedef struct shop_information {
	gint site_id;						/*所属网站编号*/
	gint shop_id;						/*店铺编号*/
	gchar shop_name[20];				/*店铺名字*/
	gchar seller_name[10];				/*负责人名字*/
	gchar seller_addr[20];				/*店铺名字*/
	gchar deposit_bank[20];				/*开户银行*/
	gint deal_amount;					/*交易次数*/
	struct shop_information *shop_next;	/*指向下个节点*/
	DealInfo *deal_head;				/*指向交易链*/
} ShopInfo;
typedef struct site_information {
	gint site_id;						/*网站编号*/
	gchar site_name[20];				/*网站名称*/
	gchar site_addr[50];				/*网站地址*/
	gint shop_amount;					/*店铺总数*/
	struct site_information *site_next;	/*指向下个节点*/
	ShopInfo *shop_head;				/*指向店铺链*/
} SiteInfo;


void AddSite(gchar (*site_data)[50]);							/*添加网站*/
gboolean SearchSite(gint search_key_id, gchar *search_key);		/*查找网站*/
void EditSite(gint selected_row, gchar (*site_data)[50]);		/*编辑网站*/
void DelSite(gint selected_row, gchar *site_id);				/*删除网站*/
		
void AddShop(gchar (*shop_data)[20]);							/*增加店铺*/																			
gboolean SearchShop(gint search_key_id, gchar *search_key);		/*查找店铺*/
void EditShop(gint selected_row, gchar *former_site_id, 
		gchar (*shop_data)[20]);								/*编辑店铺*/	
ShopInfo *DelShop(gint selected_row, gchar *str_site_id, 
		gchar *str_shop_id, gboolean real_del);					/*删除店铺*/

void AddDeal(gchar (*deal_data)[20]);							/*增加交易*/		
gboolean SearchDeal(gint search_key_id, gchar *search_key);		/*查找交易*/
void EditDeal(gint selected_row, gchar *former_shop_id,
		gchar (*deal_data)[20]);								/*编辑交易*/
DealInfo *DelDeal(gint selected_row, gchar *str_shop_id, 
		gchar *str_deal_id, gboolean real_del);					/*删除交易*/

gint RealOpen(gchar *filename);									/*读取文件到内存中*/
gint RealSave(gchar *filename);									/*将数据写入硬盘*/

void StatiSite(gchar *date);									/*统计：网站排行榜*/
void StatiShop(gchar *year);									/*统计：店铺交易情况*/
void StatiDeal(gchar (*area_name)[20]);							/*统计：地区贸易关系*/

SiteInfo *GetSite(gint site_id);								/*根据site_id查找并返回对应的SiteInfo指针*/				
ShopInfo *GetShop(gint shop_id);								/*根据shop_id查找并返回对应的ShopInfo指针*/

void RedrawShopCList(gint site_id);								/*重新构建店铺页面的clist*/
void RedrawDealCList(gint site_id, gint shop_id);				/*重新构建交易页面的clist*/
void RedrawAllCList(void);										/*重新构建所有页面的clist*/
			
void ClearList(void);											/*删除整个十字交叉链表*/						
void ChangeShowMode(gboolean expand_all);						/*修改输出模式*/
void ShowDealOfSite(SiteInfo *site_current);					/*输出某个网站下面的所有交易信息*/
void RealCheckData(void);											/*数据排错：更新所有编号，以及网站下店铺个数，店铺下交易次数*/

#endif
