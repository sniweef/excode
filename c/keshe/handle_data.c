/*
 * 数据操作模块
 *
 * 主要功能：读取、写入文件，链表节点的查询、删除等        
 */
 
#include <gtk/gtk.h>
#include <libgnomeui/gnome-messagebox.h>
#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include "create_window.h"
#include "handle_data.h"


#define SITE_COLUMN 4
#define SHOP_COLUMN 7
#define DEAL_COLUMN 6

extern GtkWidget *clist[4];		/*定义于create_window.c*/

SiteInfo *site_head = NULL;
gint site_amount = 0;				/*网站总数*/
gint shop_amount = 0;				/*店铺总数*/
gint deal_amount = 0;				/*交易总数*/
gboolean expand_all_info = TRUE; 	/*标识是否展开所有信息*/


/**********************************************************************************************************/
/**********************************************网站信息相关操作***********************************************/
/**********************************************************************************************************/
void UpdateShopAndDealData(void);				/*更新店铺、交易的编号以及店铺的所属网站编号、交易的所属店铺编号*/
void AddSite(gchar (*site_data)[50])			/*添加网站*/
{
	gint index, new_row;
	gchar *site_data_pointer[4];		/*指向site_data的指针数组*/
	
	SiteInfo *site_current = site_head;
	SiteInfo *new_site = (SiteInfo *)malloc(sizeof(SiteInfo));
	
	strcpy(new_site -> site_name, (gchar *)site_data[1]);
	strcpy(new_site -> site_addr, (gchar *)site_data[2]);
	new_site -> shop_amount = 0;
	new_site -> shop_head = NULL;
	
	g_message("new_site -> site_name: %s", new_site -> site_name);
	g_message("new_site -> site_addr: %s", new_site -> site_addr);
	
	if (site_current == NULL)		/*如果是头指针*/
	{
		g_message("site_current==NULL");		
		site_amount++;	
		new_site -> site_id = 1;
		site_head = new_site;	
		new_site -> site_next = NULL;			
	}
	else 		/*直接插入链表末尾*/
	{
		g_message("site_current!=NULL");		
		while (site_current -> site_next != NULL)
			site_current = site_current -> site_next;
			
		site_amount++;
		new_site -> site_id = site_amount;	
		site_current -> site_next = new_site;
		new_site -> site_next = NULL;
	}	
		
	sprintf(site_data[0], "%d", new_site -> site_id);
	sprintf(site_data[3], "%d", new_site -> shop_amount);
	
	for (index = 0; index < SITE_COLUMN; index++)	
		site_data_pointer[index] = site_data[index];
	
	/*在clist增加一条信息*/
	new_row = gtk_clist_append(GTK_CLIST(clist[0]), site_data_pointer);
	gtk_clist_moveto(GTK_CLIST(clist[0]), new_row, 0, 0.0, 0.0);
	gtk_clist_select_row(GTK_CLIST(clist[0]), new_row, 0);	
}
gboolean SearchSite(gint search_key_id, gchar *search_key)		/*查找网站*/
{
	g_message("SearchSite");
	g_message("search_key=%s", search_key);
	g_message("site_amount=%d", site_amount);	
	gint index;
	gchar *temp;	/*临时变量，用来存储clist中每行对应单元格的数据*/
	
	for (index = 0; index < site_amount; index++)
	{		
		gtk_clist_get_text(GTK_CLIST(clist[0]), index, search_key_id + 1, &temp);
		g_message("temp=%s", temp);
		if (!strcmp(search_key, temp))
		{
			g_message("find site");
			gtk_clist_moveto(GTK_CLIST(clist[0]), index, search_key_id + 1, 0.0, 0.0);
			gtk_clist_select_row(GTK_CLIST(clist[0]), index, search_key_id + 1);
			return TRUE;
		}
	}
	
	ShowMessageBox("未找到任何结果!");
	return FALSE;
}
void EditSite(gint selected_row, gchar (*site_data)[50])	/*编辑网站*/
{
	gint index, site_id;
	SiteInfo *site_current = site_head;
	for (index = 0; index < SITE_COLUMN; index++)
		gtk_clist_set_text(GTK_CLIST(clist[0]), selected_row, index, site_data[index]);
		
	site_id = atoi(site_data[0]);		/*将字符串转换为整型数字*/
	g_message("site_id=%d", site_id);
	
	for (index = 1; index < site_id; index++)
		site_current = site_current -> site_next;
		
	strcpy(site_current -> site_name, site_data[1]);
	strcpy(site_current -> site_addr, site_data[2]);
	
	g_message("site_current->site_name=%s",site_current -> site_name);
	g_message("site_current->site_addr=%s",site_current -> site_addr);
}
void DelSite(gint selected_row, gchar *str_site_id)		/*删除网站*/
{
	gint index, site_id = atoi(str_site_id);
	SiteInfo *site_current = site_head, *site_prior = site_current;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	g_message("site_id=%d", site_id);
	
	gtk_clist_remove(GTK_CLIST(clist[0]), selected_row);
	
	for (index = 1; index < site_id; index++)		/*根据site_id进行查找*/
	{
		site_prior = site_current;
		site_current = site_current -> site_next;
	}
	
	if (site_current == site_head)
		site_head = site_current -> site_next;
	else
		site_prior -> site_next = site_current -> site_next;		
		
	/*修改后面网站的编号*/
	if (site_head != NULL && site_prior -> site_next != NULL)
	{
		while ((site_prior = site_prior -> site_next)!= NULL)
			(site_prior -> site_id)--;
	}
	
	site_amount--;
	shop_amount -= site_current -> shop_amount;
	g_message("site_amount=%d", site_amount);
	
	/*删除site节点下的shop、deal链的数据*/
	shop_current = site_current -> shop_head;
	while (shop_current != NULL)
	{
		deal_current = shop_current -> deal_head;
		while (deal_current != NULL)
		{
			free(deal_current);
			deal_current = deal_current -> deal_next;
		}
		free(shop_current);
		shop_current = shop_current -> shop_next;
	}
	free(site_current);	
	UpdateShopAndDealData();
	
	RedrawAllCList();	/*刷新所有clist的信息*/
	
	if (selected_row > site_amount - 1)
		selected_row = site_amount - 1;
	gtk_clist_moveto(GTK_CLIST(clist[0]), selected_row, 0, 0.0, 0.0);
	gtk_clist_select_row(GTK_CLIST(clist[0]), selected_row, 0);
}
void UpdateShopAndDealData(void)		/*更新店铺、交易的编号以及店铺的所属网站编号、交易的所属店铺编号*/
{
	gint shop_count = 0, deal_count = 0; 
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	while (site_current != NULL)
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			shop_count++;
			shop_current -> shop_id = shop_count;
			shop_current -> site_id = site_current -> site_id;		
			
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				deal_count++;
				deal_current -> deal_id = deal_count;
				deal_current -> shop_id = shop_count;
				deal_current = deal_current -> deal_next;
			}			
			
			shop_current = shop_current -> shop_next;
		}
		site_current = site_current -> site_next;
	}	
}
/**********************************************************************************************************/
/***********************************************店铺信息相关操作**********************************************/
/**********************************************************************************************************/
void SearchAndEditShop(ShopInfo *shop_edited, gint selected_row, gchar (*shop_data)[20]);	/*找到并编辑店铺*/
void UpdateDealData(void);																	/*更新店铺下deal的shop_id信息*/
void UpdateSiteCList(SiteInfo *site_current, gint site_id);									/*更新网站clist中site_id对应网站的店铺个数*/

SiteInfo *GetSite(gint site_id)		/*根据site_id查找并返回对应的SiteInfo指针*/
{
	SiteInfo *site_current = site_head;
	gint index;
	
	for (index = 1; index < site_id && site_current != NULL; index++)
		site_current = site_current -> site_next;
	return site_current;
}

void AddShop(gchar (*shop_data)[20])	/*增加店铺*/
{
	g_message("AddShop");
	gint index = 1, new_row = 0;
	gchar *shop_data_pointer[7];
	gint site_id = atoi(shop_data[0]);
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current  = NULL;	
	ShopInfo *new_shop = (ShopInfo *)malloc(sizeof(ShopInfo));
	
	site_current = GetSite(site_id);
	(site_current -> shop_amount)++;
	shop_current = site_current -> shop_head;
	
	new_shop -> site_id = site_id;
	strcpy(new_shop -> shop_name, shop_data[2]);
	strcpy(new_shop -> seller_name, shop_data[3]);
	strcpy(new_shop -> seller_addr, shop_data[4]);
	strcpy(new_shop -> deposit_bank, shop_data[5]);
	
	/*插入并给店铺进行编号*/
	if (shop_current == NULL)
	{
		g_message("shop_current == NULL");
		shop_amount++;
		
		site_current -> shop_head= new_shop;
		new_shop -> shop_next = NULL;		
	}	
	else
	{	
		g_message("shop_current != NULL");
		while (shop_current -> shop_next != NULL)
			shop_current = shop_current -> shop_next;
			
		shop_amount++;				
		
		shop_current -> shop_next = new_shop;
		new_shop -> shop_next = NULL;
	}
	
	new_shop -> deal_head = NULL;
	new_shop -> shop_id = shop_amount;
	new_shop -> deal_amount = 0;
	g_message("AddShop:shop_id=%d", new_shop -> shop_id);
	sprintf(shop_data[1], "%d", new_shop -> shop_id);
	sprintf(shop_data[6], "%d", new_shop -> deal_amount);
	
	for (index = 0; index < SHOP_COLUMN; index++)
		shop_data_pointer[index] = shop_data[index];
	
	/*在clist中增加一条信息*/
	new_row = gtk_clist_append(GTK_CLIST(clist[1]), shop_data_pointer);	
	gtk_clist_moveto(GTK_CLIST(clist[1]), new_row, 0, 0.0, 0.0);
	gtk_clist_select_row(GTK_CLIST(clist[1]), new_row, 0);
	//SortCList(1);
	
			
	UpdateSiteCList(site_current, site_id);
}
gboolean SearchShop(gint search_key_id, gchar *search_key)		/*查找店铺*/
{
	g_message("SearchShop");
	g_message("search_key=%s", search_key);
	g_message("shop_amount=%d", shop_amount);	
	gint index, shop_row;
	gchar *temp;
	
	shop_row = GTK_CLIST(clist[1]) -> rows;
	for (index = 0; index < shop_row; index++)
	{		
		gtk_clist_get_text(GTK_CLIST(clist[1]), index, search_key_id + 2, &temp);
		g_message("temp=%s", temp);
		if (!strcmp(search_key, temp))
		{
			g_message("find shop");
			gtk_clist_moveto(GTK_CLIST(clist[1]), index, search_key_id + 2, 0.0, 0.0);
			gtk_clist_select_row(GTK_CLIST(clist[1]), index, search_key_id + 2);
			return TRUE;
		}
	}
	
	ShowMessageBox("未找到任何结果!");
	return FALSE;
}
void EditShop(gint selected_row, gchar *former_site_id, gchar (*shop_data)[20])	/*编辑店铺*/
{
	g_message("EditShop");
	g_message("former_site_id=%s, later_site_id=%s", former_site_id, shop_data[0]);
	ShopInfo *shop_edited = NULL;
	
	if (strcmp(former_site_id, shop_data[0]))	/*如果修改了所属网站编号*/
	{
		shop_edited = DelShop(selected_row, former_site_id, shop_data[1], FALSE);				
		SearchAndEditShop(shop_edited, selected_row, shop_data);		
		
		shop_amount++;
		//RedrawShopCList(0);
		//SortCList(1);
		RedrawDealCList(0, 0);
	}
	else	
		SearchAndEditShop(shop_edited, selected_row, shop_data);
			
}
void SearchAndEditShop(ShopInfo *shop_edited, gint selected_row, gchar (*shop_data)[20])	/*找到并编辑店铺*/
{
	g_message("SearchAndEditShop");
	gint index;
	gint site_id = atoi(shop_data[0]);
	gint shop_id = atoi(shop_data[1]);	
	
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current = NULL, *shop_prior;
	
	for (index = 0; index < SHOP_COLUMN; index++)
		gtk_clist_set_text(GTK_CLIST(clist[1]), selected_row, index, shop_data[index]);
		
	site_current = GetSite(site_id);	
	shop_prior = shop_current = site_current -> shop_head;
	
	if (shop_edited != NULL)	/*如果修改了所属网站编号*/
	{		
		if (shop_current == NULL)
		{
			g_message("shop_current == NULL");
			site_current -> shop_head = shop_edited;
			(shop_edited) -> shop_next = NULL;
		}
		else
		{
			while (shop_current != NULL)
			{
				if (shop_current -> shop_id > shop_id)	/*插在当前节点的前面*/
				{
					if (shop_current == site_current -> shop_head)	/*如果是第一个节点*/
					{
						site_current -> shop_head = shop_edited;
						shop_edited -> shop_next = shop_current;
					}
					else
					{
						shop_prior -> shop_next = shop_edited;
						shop_edited -> shop_next = shop_current;
					}
					break;
				}
				shop_prior = shop_current;
				shop_current = shop_current -> shop_next;
			}
			if (shop_current == NULL && shop_prior -> shop_id < shop_id)	/*如果须插在最后节点的后面，必须再进行一次判断*/
			{
				shop_prior -> shop_next = shop_edited;
				shop_edited -> shop_next = NULL;
			}		
			
		}
		shop_current = shop_edited;
		UpdateSiteCList(site_current, site_id);
	}
	else	/*未修改所属网站编号*/
	{
		g_message("shop_edited == NULL");
		while (shop_current != NULL)
		{
			if (shop_current -> shop_id == shop_id)
				break;
			shop_current = shop_current -> shop_next;
		}
	}
	
	/*修改对应数据*/
	shop_current -> site_id = atoi(shop_data[0]);
	strcpy(shop_current -> shop_name, shop_data[2]);
	strcpy(shop_current -> seller_name, shop_data[3]);
	strcpy(shop_current -> seller_addr, shop_data[4]);
	strcpy(shop_current -> deposit_bank, shop_data[5]);
}
ShopInfo *DelShop(gint selected_row, gchar *str_site_id, gchar *str_shop_id, gboolean real_del)		/*删除店铺*/
{
	g_message("DelShop");
	gint site_id = atoi(str_site_id);
	gint shop_id = atoi(str_shop_id);
	g_message("site_id=%d", site_id);
	g_message("shop_id=%d", shop_id);		
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current = NULL, *shop_prior;	
	DealInfo *deal_current;
	
	shop_amount--;
	
	site_current = GetSite(site_id);
	
	(site_current -> shop_amount)--;
	shop_prior = shop_current = site_current -> shop_head;
	
	if (shop_current -> shop_id < shop_id)		/*如果第一个节点的shop_id小于欲删除的商家的shop_id,就继续往后找*/
		do
		{
			shop_prior = shop_current;
			shop_current = shop_current -> shop_next;
		} while (shop_current -> shop_id < shop_id);
	
	if (shop_current == site_current -> shop_head)
		site_current -> shop_head = shop_current -> shop_next;
	else
		shop_prior -> shop_next = shop_current -> shop_next ;

	UpdateSiteCList(site_current, site_id);
	
	if (real_del)	/*如果要彻底删除,刚会修改后面商家编号并释放相应内存*/	
	{	
		gtk_clist_remove(GTK_CLIST(clist[1]), selected_row);
		deal_amount -= shop_current -> deal_amount;
		
		deal_current = shop_current -> deal_head;
		while (deal_current != NULL)
		{
			free(deal_current);
			deal_current = deal_current -> deal_next;
		}
		free(shop_current);	
		UpdateDealData();
		
		/*修改后面商家的编号*/
		site_current = site_head;
		while (site_current != NULL)
		{	
			shop_current = site_current -> shop_head;
			while (shop_current != NULL)
			{
				if (shop_current -> shop_id > shop_id)
					(shop_current -> shop_id)--;
				shop_current = shop_current -> shop_next;
			}	
			site_current = site_current -> site_next;			
		}
		
		RedrawShopCList(site_id);
		RedrawDealCList(0, 0);
		
		if (selected_row > shop_amount - 1)
			selected_row = shop_amount - 1;
		gtk_clist_moveto(GTK_CLIST(clist[1]), selected_row, 0, 0.0, 0.0);
		gtk_clist_select_row(GTK_CLIST(clist[1]), selected_row, 0);	
		return NULL;
	}	
	else
		return shop_current;
}
void RedrawShopCList(gint site_id)	/*重新构建ShopCList*/
{
	g_message("RedrawShopCList");
	gchar shop_data[SHOP_COLUMN][20];
	gchar *shop_data_pointer[SHOP_COLUMN];
	gint index = 1;
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current = NULL;
	
	for (index = 0; index < SHOP_COLUMN; index++)
		shop_data_pointer[index] = shop_data[index];
	
	gtk_clist_clear(GTK_CLIST(clist[1]));		/*清空CList*/
	gtk_clist_freeze(GTK_CLIST(clist[1]));		/*冻结CList以防止屏幕闪烁*/
	
	if (expand_all_info)		/*如果是“展开全部”模式*/
	{
		while (site_current != NULL)
		{	
			shop_current = site_current -> shop_head;		
			while (shop_current != NULL)
			{
				sprintf(shop_data[0], "%d", shop_current -> site_id);
				sprintf(shop_data[1], "%d", shop_current -> shop_id);
				strcpy(shop_data[2], shop_current -> shop_name);
				strcpy(shop_data[3], shop_current -> seller_name);
				strcpy(shop_data[4], shop_current -> seller_addr);
				strcpy(shop_data[5], shop_current -> deposit_bank);
				sprintf(shop_data[6], "%d", shop_current -> deal_amount);
				
				gtk_clist_append(GTK_CLIST(clist[1]), shop_data_pointer);
				shop_current = shop_current -> shop_next;
			}
			site_current = site_current -> site_next;
		}
	}
	else		/*“收起其他”模式，只输出所属网站编号为site_id的商家*/
	{
		if (site_id <= 0)
			return;
		for (index = 1; index < site_id; index++)
			site_current = site_current -> site_next;
		shop_current = site_current -> shop_head;	
			
		while (shop_current != NULL)
		{
			sprintf(shop_data[0], "%d", shop_current -> site_id);
			sprintf(shop_data[1], "%d", shop_current -> shop_id);
			strcpy(shop_data[2], shop_current -> shop_name);
			strcpy(shop_data[3], shop_current -> seller_name);
			strcpy(shop_data[4], shop_current -> seller_addr);
			strcpy(shop_data[5], shop_current -> deposit_bank);
			sprintf(shop_data[6], "%d", shop_current -> deal_amount);
				
			gtk_clist_append(GTK_CLIST(clist[1]), shop_data_pointer);
			shop_current = shop_current -> shop_next;
		}
	}
	
	//SortCList(1);
	
	gtk_clist_thaw(GTK_CLIST(clist[1]));
}
void UpdateSiteCList(SiteInfo *site_current,gint site_id)	/*更新网站clist中site_id对应网站的店铺个数*/
{
	g_message("UpdateSiteCList");
	gint shop_count = 0, index, row = -1;
	gchar shop_amount_one_site[10], *temp;
	ShopInfo *shop_current = site_current -> shop_head;
	
	/*计数*/
	while (shop_current != NULL)
	{
		shop_count++;
		shop_current = shop_current -> shop_next;
	}
	site_current -> shop_amount = shop_count;
	sprintf(shop_amount_one_site, "%d", shop_count);
	
	g_message("shop_amount_one_site=%s", shop_amount_one_site);
	
	for (index = 0; index < site_amount; index++)
	{
		gtk_clist_get_text(GTK_CLIST(clist[0]), index, 0, &temp);
		if (site_id == atoi(temp))
			row = index;
	}
	if (row != -1)
		gtk_clist_set_text(GTK_CLIST(clist[0]), row, 3, shop_amount_one_site);
}
void UpdateDealData(void)	/*更新店铺下deal的shop_id信息*/
{
	gint count = 0; 
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	while (site_current != NULL)
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				count++;
				deal_current -> deal_id = count;
				deal_current -> shop_id = shop_current -> shop_id;
				deal_current = deal_current -> deal_next;
			}			
			shop_current = shop_current -> shop_next;
		}
		site_current = site_current -> site_next;
	}	
}
/**********************************************************************************************************/
/**********************************************交易信息相关操作***********************************************/
/**********************************************************************************************************/
void UpdateShopCList(ShopInfo *shop_current,gint shop_id);			/*更新店铺clist中shop_id对应的店铺的交易个数*/
void SearchAndEditDeal(DealInfo *deal_edited, gint selected_row, gchar (*deal_data)[20]);	/*找到并修改交易*/

ShopInfo *GetShop(gint shop_id)		/*根据shop_id找到并返回对应的ShopInfo指针*/
{
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	gboolean result = FALSE;
	
	while (site_current != NULL && !result)	
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			if (shop_current -> shop_id == shop_id)
			{
				result = TRUE;
				break;
			}
			shop_current = shop_current -> shop_next;
		}
		site_current = site_current -> site_next;
	}
	
	if (!result)	
		return NULL;
	return shop_current;
}
void AddDeal(gchar (*deal_data)[20])		/*增加交易*/
{
	gint shop_id = atoi(deal_data[0]);
	gint new_row, index;
	
	gchar *deal_data_pointer[DEAL_COLUMN];
	ShopInfo *shop_current = NULL;
	DealInfo *deal_current = NULL, *new_deal;
	
	
	shop_current = GetShop(shop_id);
	(shop_current -> deal_amount)++;
		
	deal_current = shop_current -> deal_head;
	
	new_deal = (DealInfo *)malloc(sizeof(DealInfo));	
	new_deal -> shop_id = shop_id;
	new_deal -> pay_type = atoi(deal_data[2]);
	new_deal -> deal_money = atof(deal_data[3]);
	strcpy(new_deal -> deal_date, deal_data[4]);
	strcpy(new_deal -> customer_addr, deal_data[5]);
	
	/*为交易编号*/
	if (deal_current == NULL)
	{
		deal_amount++;
		
		shop_current -> deal_head = new_deal;
		new_deal -> deal_next = NULL;
	}
	else
	{
		deal_amount++;
		
		while (deal_current -> deal_next != NULL)
			deal_current = deal_current -> deal_next;
		deal_current -> deal_next = new_deal;
		new_deal -> deal_next = NULL;
	}
	new_deal -> deal_id = deal_amount;
	
	g_message("AddDeal:deal_id=%d", new_deal -> deal_id);
	sprintf(deal_data[1], "%d", new_deal -> deal_id);
	
	for (index = 0; index < DEAL_COLUMN; index++)
		deal_data_pointer[index] = deal_data[index];
	
	new_row = gtk_clist_append(GTK_CLIST(clist[2]), deal_data_pointer);
	gtk_clist_moveto(GTK_CLIST(clist[2]), new_row, 0, 0.0, 0.0);
	gtk_clist_select_row(GTK_CLIST(clist[2]), new_row, 0);
	
	UpdateShopCList(shop_current, shop_id);
	//SortCList(2);
}
gboolean SearchDeal(gint search_key_id, gchar *search_key)		/*查找交易*/
{
	g_message("SearchShop");
	g_message("search_key=%s", search_key);
	g_message("deal_amount=%d", deal_amount);	
	gint index, deal_row;
	gchar *temp;
	
	deal_row = GTK_CLIST(clist[2]) -> rows;
	
	for (index = 0; index < deal_row; index++)
	{		
		gtk_clist_get_text(GTK_CLIST(clist[2]), index, search_key_id + 4, &temp);
		g_message("temp=%s", temp);
		if (!strcmp(search_key, temp))
		{
			g_message("find deal");
			gtk_clist_moveto(GTK_CLIST(clist[2]), index, search_key_id + 4, 0.0, 0.0);
			gtk_clist_select_row(GTK_CLIST(clist[2]), index, search_key_id + 4);
			return TRUE;
		}
	}
	
	ShowMessageBox("未找到任何结果!");
	return FALSE;
}
void EditDeal(gint selected_row, gchar *former_shop_id, gchar (*deal_data)[20])		/*编辑交易*/
{
	g_message("EditDeal");
	DealInfo *deal_edited = NULL;
	
	if (strcmp(former_shop_id, deal_data[0]))	/*如果修改了所属商家编号*/
	{
		deal_edited = DelDeal(selected_row, former_shop_id, deal_data[1], FALSE);				
		SearchAndEditDeal(deal_edited, selected_row, deal_data);		
		//RedrawDealCList(0,0);
		deal_amount++;
		//SortCList(2);
		
	}
	else
		SearchAndEditDeal(deal_edited, selected_row, deal_data);
}
void SearchAndEditDeal(DealInfo *deal_edited, gint selected_row, gchar (*deal_data)[20])		/*找到修改交易信息*/
{
	g_message("SearchAndEditDeal");
	gint index;
	gint shop_id = atoi(deal_data[0]);
	gint deal_id = atoi(deal_data[1]);	
	ShopInfo *shop_current;
	DealInfo *deal_current, *deal_prior;
	
	for (index = 0; index < DEAL_COLUMN; index++)
		gtk_clist_set_text(GTK_CLIST(clist[2]), selected_row, index, deal_data[index]);
		
	shop_current = GetShop(shop_id);
	
	deal_current = shop_current -> deal_head;
	
	if (deal_edited != NULL)	/*如果修改了所属商家编号*/
	{
		(shop_current -> deal_amount)++;
		if (deal_current == NULL)
		{
			g_message("deal_current == NULL");
			shop_current -> deal_head = deal_edited;
			deal_edited -> deal_next = NULL;
		}
		else
		{
			while (deal_current != NULL)
			{
				if (deal_current -> deal_id > deal_id)		/*找到插入的地方*/
				{
					if (deal_current == shop_current -> deal_head)	/*如果是第一个节点*/
					{
						shop_current -> deal_head = deal_edited;
						deal_edited -> deal_next = deal_current;
					}
					else
					{
						deal_prior -> deal_next = deal_edited;
						deal_edited -> deal_next = deal_current;
					}
					break;
				}
				deal_prior = deal_current;
				deal_current = deal_current -> deal_next;
			}
			if (deal_current == NULL && deal_prior -> deal_id < deal_id)	/*插在最后节点的后面*/
			{
				deal_prior -> deal_next = deal_edited;
				deal_edited -> deal_next = NULL;
			}
		}
		deal_current = deal_edited;
		UpdateShopCList(shop_current, shop_id);
	}
	else
	{
		if (deal_current -> deal_id < deal_id)		/*如果第一个节点的deal_id小于欲修改的交易的deal_id,就继续往后找*/
			do
			{
				deal_current = deal_current -> deal_next;
			} while (deal_current -> deal_id < deal_id && deal_current != NULL);
	}
	
	deal_current -> shop_id = atoi(deal_data[0]);
	deal_current -> pay_type = atoi(deal_data[2]);
	deal_current -> deal_money = atoi(deal_data[3]);
	strcpy(deal_current -> deal_date, deal_data[4]);
	strcpy(deal_current -> customer_addr, deal_data[5]);
}
DealInfo *DelDeal(gint selected_row, gchar *str_shop_id, gchar *str_deal_id, gboolean real_del)		/*删除交易*/
{
	g_message("DelShop");
	gint site_id = 0;
	gint shop_id = atoi(str_shop_id);
	gint deal_id = atoi(str_deal_id);
	g_message("shop_id=%d", shop_id);
	g_message("deal_id=%d", deal_id);		
	SiteInfo *site_current;
	ShopInfo *shop_current = NULL;	
	DealInfo *deal_current = NULL, *deal_prior;
	
	deal_amount--;
	
	shop_current = GetShop(shop_id);
	(shop_current -> deal_amount)--;
	
	deal_prior = deal_current = shop_current -> deal_head;	
	if (deal_current -> deal_id < deal_id)		/*如果第一个节点的deal_id小于欲删除的交易的deal_id,就继续往后找*/
		do
		{
			deal_prior = deal_current;
			deal_current = deal_current -> deal_next;
		} while (deal_current -> deal_id < deal_id);
	
	if (deal_current == shop_current -> deal_head)
		shop_current -> deal_head = deal_current -> deal_next;
	else
		deal_prior -> deal_next = deal_current -> deal_next ;

	UpdateShopCList(shop_current, shop_id);
	
	if (real_del)	/*如果要彻底删除,刚会修改后面交易编号并释放相应内存*/	
	{	
		gtk_clist_remove(GTK_CLIST(clist[2]), selected_row);
		
		free(deal_current);	
		site_current = site_head;
		
		/*修改后面交易的编号*/
		while (site_current != NULL)
		{
			g_message("site_current != NULL");
			shop_current = site_current -> shop_head;
			while (shop_current != NULL)
			{	
				g_message("shop_current != NULL");		
				deal_current = shop_current -> deal_head;
				while (deal_current != NULL)
				{
					if (deal_current -> deal_id > deal_id)
						(deal_current -> deal_id)--;
					deal_current = deal_current -> deal_next;
				}
				shop_current = shop_current -> shop_next;
			}
			site_current = site_current -> site_next;			
		}
		
		RedrawDealCList(site_id, shop_id);
		if (selected_row > deal_amount - 1)
			selected_row = deal_amount - 1;
		gtk_clist_moveto(GTK_CLIST(clist[2]), selected_row, 0, 0.0, 0.0);
		gtk_clist_select_row(GTK_CLIST(clist[2]), selected_row, 0);	
		return NULL;
	}	
	else
		return deal_current;
}
void RedrawDealCList(gint site_id, gint shop_id)	/*重新构建交易页面的clist*/
{
	g_message("RedrawDealCList");
	gchar deal_data[DEAL_COLUMN][20];
	gchar *deal_data_pointer[DEAL_COLUMN];
	gint index = 1;
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current = NULL;
	DealInfo *deal_current = NULL;
	
	for (index = 0; index < DEAL_COLUMN; index++)
		deal_data_pointer[index] = deal_data[index];
	
	gtk_clist_clear(GTK_CLIST(clist[2]));
	gtk_clist_freeze(GTK_CLIST(clist[2]));
	
	if (expand_all_info)		/*"展开全部"模式*/
	{
		while (site_current != NULL)
		{	
			shop_current = site_current -> shop_head;		
			while (shop_current != NULL)
			{
				deal_current = shop_current -> deal_head;
				while (deal_current != NULL)
				{
					sprintf(deal_data[0], "%d", deal_current -> shop_id);
					sprintf(deal_data[1], "%d", deal_current -> deal_id);
					sprintf(deal_data[2], "%d", deal_current -> pay_type);
					sprintf(deal_data[3], "%.2f", deal_current -> deal_money);
					strcpy(deal_data[4], deal_current -> deal_date);
					strcpy(deal_data[5], deal_current -> customer_addr);
				
					gtk_clist_append(GTK_CLIST(clist[2]), deal_data_pointer);
					deal_current = deal_current -> deal_next;
				}
				shop_current = shop_current -> shop_next;
				
			}
			site_current = site_current -> site_next;
		}
	}
	else		/*"收起其他"模式*/
	{
		if (site_id <= 0 || shop_id <= 0)
			return;
		for (index = 1; index < site_id; index++)
			site_current = site_current -> site_next;
		shop_current = site_current -> shop_head;	
			
		while (shop_current != NULL)
		{
			if (shop_current -> shop_id == shop_id)
			{
				deal_current = shop_current -> deal_head;
				while (deal_current != NULL)
				{
					sprintf(deal_data[0], "%d", deal_current -> shop_id);
					sprintf(deal_data[1], "%d", deal_current -> deal_id);
					sprintf(deal_data[2], "%d", deal_current -> pay_type);
					sprintf(deal_data[3], "%.2f", deal_current -> deal_money);
					strcpy(deal_data[4], deal_current -> deal_date);
					strcpy(deal_data[5], deal_current -> customer_addr);
				
					gtk_clist_append(GTK_CLIST(clist[2]), deal_data_pointer);
					deal_current = deal_current -> deal_next;
				}
				break;
			}
			shop_current = shop_current -> shop_next;
		}
	}
	//gtk_clist_select_row(GTK_CLIST(clist[2]), 0, 0);	
	//SortCList(2);
	
	gtk_clist_thaw(GTK_CLIST(clist[2]));
}
void UpdateShopCList(ShopInfo *shop_current,gint shop_id)	/*更新店铺clist中shop_id对应的店铺的交易个数*/
{
	g_message("UpdateShopCList");
	gint deal_count = 0, index, row;
	gchar deal_amount_one_shop[10], *temp;
	DealInfo *deal_current = shop_current -> deal_head;
	
	while (deal_current != NULL)
	{
		deal_count++;
		deal_current = deal_current -> deal_next;
	}
	shop_current -> deal_amount = deal_count;
	sprintf(deal_amount_one_shop, "%d", deal_count);
	
	g_message("deal_amount_one_shop=%s",deal_amount_one_shop);
	for (index = 0; index < shop_amount; index++)
	{
		gtk_clist_get_text(GTK_CLIST(clist[1]), index, 1, &temp);
		if (shop_id == atoi(temp))
			row = index;
	}
	if (row != -1)
		gtk_clist_set_text(GTK_CLIST(clist[1]), row, 6, deal_amount_one_shop);
}
/**********************************************************************************************************/
/*************************************************文件相关操作***********************************************/
/**********************************************************************************************************/

gint RealOpen(gchar *site_file_name)		/*读取文件到内存中*/
{
	g_message("RealOpen");
	FILE *site_file, *shop_file, *deal_file;		/*site_file存储site链表的信息，shop_file存储shop链表的信息，deal_file存储deal链表的信息*/
	gchar shop_file_name[55], deal_file_name[55];	
	SiteInfo *site_new, *site_current;
	ShopInfo *shop_new, *shop_current;
	DealInfo *deal_new, *deal_current;
	gint index, index2;
	
	/*将site_file_name后面接上"_shop"作为shop_file的文件名字，接上"_deal"作为deal_file的文件名字*/
	strcpy(shop_file_name, site_file_name);
	strcpy(deal_file_name, site_file_name);
	strcat(shop_file_name, "_shop");
	strcat(deal_file_name, "_deal");
	g_message("site_file_name=%s\n\tshop_file_name=%s\n\tdeal_file_name=%s", site_file_name, shop_file_name, deal_file_name);	
	
	site_file = fopen(site_file_name, "r");
	shop_file = fopen(shop_file_name, "r");
	deal_file = fopen(deal_file_name, "r");
	
	if (site_file  == NULL || shop_file == NULL || deal_file == NULL)		/*如果三个文件有一个打开失败，即退出函数并提示打开失败*/
	{
		ShowMessageBox("打开文件失败!不支持的数据文件！");
		return 0;
	}
	
	site_head = NULL;
	expand_all_info = TRUE;
	site_amount = shop_amount = deal_amount = 0;
	
	while (!feof(site_file))
	{
		site_new = (SiteInfo *)malloc(sizeof(SiteInfo));
		fread(site_new, sizeof(SiteInfo), 1, site_file);	
		if(!feof(site_file))
		{
			site_amount++;
			if (site_head == NULL)
				site_current = site_head = site_new;			
			else
			{
				site_current -> site_next = site_new;
				site_current = site_new;
			}
			site_current -> shop_head = NULL;
			site_current -> site_next = NULL;			
			
			g_message("site_current -> shop_amount=%d", site_current -> shop_amount);
			for (index = 0; index < (site_current -> shop_amount); index++)		/*根据site节点里面的成员shop_amount决定读取多少个shop节点*/
			{
				shop_amount++;
				shop_new = (ShopInfo *)malloc(sizeof(ShopInfo));
				fread(shop_new, sizeof(ShopInfo), 1, shop_file);
				if (site_current -> shop_head == NULL)
					shop_current = site_current -> shop_head = shop_new;			
				else
				{
					shop_current -> shop_next = shop_new;
					shop_current = shop_new;
				}
				shop_current -> deal_head = NULL;
				shop_current -> shop_next = NULL;
			
				g_message("shop_current -> deal_amount=%d", shop_current -> deal_amount);
				for (index2 = 0; index2 < (shop_current -> deal_amount); index2++)	/*根据shop节点里面的成员deal_amount决定读取多少个deal节点*/
				{
					deal_amount++;
					deal_new = (DealInfo *)malloc(sizeof(DealInfo));
					fread(deal_new, sizeof(DealInfo), 1, deal_file);
					if (shop_current -> deal_head == NULL)
						deal_current = shop_current -> deal_head = deal_new;			
					else
					{
						deal_current -> deal_next = deal_new;
						deal_current = deal_new;
					}
					deal_current -> deal_next = NULL;	
				}
			}			
		}
	}
	
	RedrawAllCList();	/*重构所有CList*/
	fclose(site_file);
	fclose(shop_file);
	fclose(deal_file);
	
	return 1;
}
gint RealSave(gchar *site_file_name)		/*将数据写入硬盘*/
{	
	g_message("RealSave");
	FILE *site_file, *shop_file, *deal_file;
	gchar shop_file_name[55], deal_file_name[55];
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	strcpy(shop_file_name, site_file_name);
	strcpy(deal_file_name, site_file_name);
	strcat(shop_file_name, "_shop");
	strcat(deal_file_name, "_deal");
	g_message("site_file_name=%s\n\tshop_file_name=%s\n\tdeal_file_name=%s", site_file_name, shop_file_name, deal_file_name);	
	
	site_file = fopen(site_file_name, "w");
	shop_file = fopen(shop_file_name, "w");
	deal_file = fopen(deal_file_name, "w");
	
	if (site_file  == NULL || shop_file == NULL || deal_file == NULL)
	{
		ShowMessageBox("创建文件失败!");
		return 0;
	}
	
	while (site_current != NULL)
	{
		fwrite(site_current, sizeof(SiteInfo), 1, site_file);
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			fwrite(shop_current, sizeof(ShopInfo), 1, shop_file);
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				fwrite(deal_current, sizeof(DealInfo), 1, deal_file);
				deal_current = deal_current -> deal_next;
			}			
			shop_current = shop_current -> shop_next;
		}
		site_current = site_current -> site_next;
	}
	
	fclose(site_file);
	fclose(shop_file);
	fclose(deal_file);
	
	return 1;
}

void RedrawAllCList(void)		/*重新构建所有页面的clist*/
{
	g_message("RedrawAllCList");
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	gint index;
	gchar site_data[SITE_COLUMN][50];
	gchar shop_data[SHOP_COLUMN][20];
	gchar deal_data[DEAL_COLUMN][20];
	gchar *site_data_pointer[SITE_COLUMN];	
	gchar *shop_data_pointer[SHOP_COLUMN];
	gchar *deal_data_pointer[DEAL_COLUMN];
	
	for (index = 0; index < 3; index++)
	{
		gtk_clist_clear(GTK_CLIST(clist[index]));
		gtk_clist_freeze(GTK_CLIST(clist[index]));
	}
	
	/*初始化三个指针数组*/
	for (index = 0; index < SITE_COLUMN; index++)
		site_data_pointer[index] = site_data[index];
	for (index = 0; index < SHOP_COLUMN; index++)
		shop_data_pointer[index] = shop_data[index];
	for (index = 0; index < DEAL_COLUMN; index++)
		deal_data_pointer[index] = deal_data[index];
		
	while (site_current != NULL)
	{	
		g_message("site_current != NULL");
		/*增加一条site信息*/
		sprintf(site_data[0], "%d", site_current -> site_id);
		strcpy(site_data[1], site_current -> site_name);
		strcpy(site_data[2], site_current -> site_addr);
		sprintf(site_data[3],"%d",site_current -> shop_amount);
		gtk_clist_append(GTK_CLIST(clist[0]), site_data_pointer);		
		
		shop_current = site_current -> shop_head;		
		while (shop_current != NULL)
		{
			g_message("shop_current != NULL");
			/*增加一条shop信息*/
			sprintf(shop_data[0], "%d", shop_current -> site_id);
			sprintf(shop_data[1], "%d", shop_current -> shop_id);
			strcpy(shop_data[2], shop_current -> shop_name);
			strcpy(shop_data[3], shop_current -> seller_name);
			strcpy(shop_data[4], shop_current -> seller_addr);
			strcpy(shop_data[5], shop_current -> deposit_bank);
			sprintf(shop_data[6], "%d", shop_current -> deal_amount);
			
			gtk_clist_append(GTK_CLIST(clist[1]), shop_data_pointer);
				
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				g_message("deal_current != NULL");
				/*增加一条deal信息*/
				sprintf(deal_data[0], "%d", deal_current -> shop_id);
				sprintf(deal_data[1], "%d", deal_current -> deal_id);
				sprintf(deal_data[2], "%d", deal_current -> pay_type);
				sprintf(deal_data[3], "%.2f", deal_current -> deal_money);
				strcpy(deal_data[4], deal_current -> deal_date);
				strcpy(deal_data[5], deal_current -> customer_addr);
			
				gtk_clist_append(GTK_CLIST(clist[2]), deal_data_pointer);
				deal_current = deal_current -> deal_next;
			}
			shop_current = shop_current -> shop_next;
			
		}
		site_current = site_current -> site_next;		
	}
	
	for (index = 0; index < 3; index++)
	{
		//SortCList(index);
		gtk_clist_thaw(GTK_CLIST(clist[index]));
	}		
	
	gtk_clist_moveto(GTK_CLIST(clist[0]), 0, 0, 0.0, 0.0);
	gtk_clist_select_row(GTK_CLIST(clist[0]), 0, 0);
	
}

/**********************************************************************************************************/
/*************************************************统计模块**************************************************/
/*********************************************************************************************************/
#define STATI_SITE 0
#define STATI_SHOP 1
#define STATI_DEAL 2

void ShowStatiSiteResult(gchar (*stati_result)[5][20], gchar *date); 	/*输出统计网站后的结果*/
void JudgeStatiDealResult(gchar *stati_result, gfloat balance);			/*根据统计后结果判断贸易关系(顺差或逆差)*/

void StatiSite(gchar *date)		/*统计：网站排行榜*/
{
	gchar date_temp[7];
	gint deal_amount_one_site[site_amount];		/*记录每个网站下的交易次数*/
	gint site_id[site_amount];					/*记录网站编号，与上个数组的数据同步，即元素编号一致的代表同个网站*/
	gint index, index2, temp;
	gchar stati_result[5][5][20];				/*记录统计结果，即5个商家的所有相关数据*/
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	for (index = 0; index < site_amount; index++)
	{
		deal_amount_one_site[index] = 0;
		site_id[index] = index + 1;
	}
	
	index = 0;
	while (site_current != NULL)
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				strncpy(date_temp, deal_current -> deal_date, 6);	/*截取日期的前6位作为月份*/
				date_temp[6] = '\0';
				
				if (!strcmp(date_temp, date))
					deal_amount_one_site[index]++;
					
				deal_current = deal_current -> deal_next;
			}
			shop_current = shop_current -> shop_next;	
		}
		site_current = site_current -> site_next;	
		index++;
	}
	
	/*排序，同时更新数组site_id使之与数组deal_amount_one_site同步*/
	for (index = 0; index < site_amount; index++)
	{
		for (index2 = index + 1; index2 < site_amount; index2++)
		{
			if (deal_amount_one_site[index] < deal_amount_one_site[index2])
			{
				temp = deal_amount_one_site[index];
				deal_amount_one_site[index] = deal_amount_one_site[index2];
				deal_amount_one_site[index2] = temp;
				
				temp = site_id[index];
				site_id[index] = site_id[index2];
				site_id[index2] = temp;
			}
		}
	}

	/*存储统计结果*/
	for (index = 0; index < site_amount && index < 5; index++)
	{
		site_current = GetSite(site_id[index]);
		sprintf(stati_result[index][0], "%d", index + 1);
		sprintf(stati_result[index][1], "%d", site_id[index]);
		strcpy(stati_result[index][2], site_current -> site_name);
		strcpy(stati_result[index][3], date);
		sprintf(stati_result[index][4], "%d", deal_amount_one_site[index]);
	}
	ShowStatiSiteResult(stati_result, date);
}
void StatiShop(gchar *year)		/*统计：店铺交易情况*/
{	
	g_message("StatiShop:year=%s", year);
	gchar date_temp[6];
	gint deal_amount_one_shop = 0, index;
	gfloat deal_money_amount = 0;
	gchar stati_result[5][20];
	gchar *stati_result_pointer[5];
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	CreateStatiNotebook(STATI_SHOP);
	for (index = 0; index < 5; index++)
		stati_result_pointer[index] =  stati_result[index];
	
	while (site_current != NULL)
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				strncpy(date_temp, deal_current -> deal_date, 4);	/*截取日期前4位作为年份*/
				date_temp[4] = '\0';
				
				if (!strcmp(date_temp, year))
				{
					deal_amount_one_shop++;
					deal_money_amount += deal_current -> deal_money;
				}
					
				deal_current = deal_current -> deal_next;
			}
			sprintf(stati_result[0], "%d", shop_current -> shop_id);
			strcpy(stati_result[1], shop_current -> shop_name);
			strcpy(stati_result[2], year);
			sprintf(stati_result[3], "%d", deal_amount_one_shop);
			sprintf(stati_result[4], "%.2f", deal_money_amount);
			
			gtk_clist_append(GTK_CLIST(clist[3]), stati_result_pointer);
			
			deal_amount_one_shop = 0;
			deal_money_amount = 0;
			shop_current = shop_current -> shop_next;	
		}
		site_current = site_current -> site_next;
	}
}
void StatiDeal(gchar (*stati_data)[20])
{
	gchar stati_result[5][20];
	gchar *stati_result_pointer[5];
	gfloat area_deal_money[2] = {0, 0};	/*第一个元素记录地区1资金流入金额，第二个记录地区1资金流出金额*/
	gchar date_temp[7];
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	gboolean area1_exist = FALSE, area2_exist = FALSE;		/*标识是否存在地区1、2的信息*/
	gint index;		
		
	for (index = 0; index < 5; index++)
		stati_result_pointer[index] =  stati_result[index];
	
	while (site_current != NULL)
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			if (!strcmp((gchar *)stati_data, shop_current -> seller_addr))	/*如果找到地区1的商家*/
			{	
				deal_current = shop_current -> deal_head;
				while (deal_current != NULL)
				{
					strncpy(date_temp, deal_current -> deal_date, 6);
					date_temp[6] = '\0';
					if (!strcmp((gchar *)stati_data[1], deal_current -> customer_addr) && !strcmp((gchar *)stati_data[2], date_temp))
					{
						/*地区2的买家*/
						area_deal_money[0] += deal_current -> deal_money;
						area2_exist = TRUE;
					}
					deal_current = deal_current -> deal_next;
				}
				area1_exist = TRUE;
			}	
			else if (!strcmp((gchar *)stati_data[1], shop_current -> seller_addr))	/*如果找到地区2的商家*/
			{
				deal_current = shop_current -> deal_head;
				while (deal_current != NULL)
				{
					strncpy(date_temp, deal_current -> deal_date, 6);
					date_temp[6] = '\0';
					if (!strcmp((gchar *)stati_data, deal_current -> customer_addr) && !strcmp((gchar *)stati_data[2], date_temp))
					{
						/*地区1的买家*/
						area1_exist = TRUE;
						area_deal_money[1] += deal_current -> deal_money;
					}
					
					deal_current = deal_current -> deal_next;					
				}
				area2_exist = TRUE;
			}
			
			shop_current = shop_current -> shop_next;	
		}
		site_current = site_current -> site_next;	
	}
	
	if (!area1_exist || !area2_exist)		
	{
		ShowMessageBox("地区1、2之间在指定月份无贸易关系！");
		return;
	}
	
	CreateStatiNotebook(STATI_DEAL);
	
	/*输出地区1的情况*/
	strcpy(stati_result[0], stati_data[0]);	
	strcpy(stati_result[1], stati_data[2]);
	sprintf(stati_result[2], "%.2f", area_deal_money[0]);
	sprintf(stati_result[3], "%.2f", area_deal_money[1]);
	JudgeStatiDealResult(stati_result[4], area_deal_money[0]-area_deal_money[1]);
	gtk_clist_append(GTK_CLIST(clist[3]), stati_result_pointer);
	
	/*输出地区2的情况*/
	strcpy(stati_result[0], stati_data[1]);	
	sprintf(stati_result[2], "%.2f", area_deal_money[1]);
	sprintf(stati_result[3], "%.2f", area_deal_money[0]);
	JudgeStatiDealResult(stati_result[4], area_deal_money[1]-area_deal_money[0]);
	gtk_clist_append(GTK_CLIST(clist[3]), stati_result_pointer);
}

void ShowStatiSiteResult(gchar (*stati_result)[5][20], gchar *date)		/*输出统计网站后的结果*/
{
	gchar *stati_result_pointer[5];
	gint index, index2;
	
	CreateStatiNotebook(STATI_SITE);
	for (index = 0; index < 5 && index < site_amount; index++)
	{
		for (index2 = 0; index2 < 5; index2++)
			stati_result_pointer[index2] = *(*(stati_result + index) + index2);			
		
		gtk_clist_append(GTK_CLIST(clist[3]), stati_result_pointer);
	}
}	
void JudgeStatiDealResult(gchar *stati_result, gfloat balance)		/*根据统计后结果判断贸易关系(顺差或逆差)*/
{
	if (balance == 0)
		strcpy(stati_result, "平衡");
	else if (balance > 0)
		sprintf(stati_result, "顺差：%.2f", balance);
	else
		sprintf(stati_result, "逆差：%.2f", balance);	
}


/**********************************************************************************************************/
/*************************************************其他函数**************************************************/
/*********************************************************************************************************/
void ClearList(void)				/*删除整个十字交叉链表*/	
{
	g_message("ClearList");
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	while (site_current != NULL)
	{
		shop_current = site_current -> shop_head;
		while (shop_current != NULL)
		{
			deal_current = shop_current -> deal_head;
			while (deal_current != NULL)
			{
				free(deal_current);
				
				g_message("deal_current != NULL");
				deal_current = deal_current -> deal_next;
			}			
			free(shop_current);
			g_message("shop_current != NULL");
			shop_current = shop_current -> shop_next;
		}
		free(site_current);
		g_message("site_current != NULL");
		site_current = site_current -> site_next;
	}
	site_head = NULL;
	site_amount = 0;
	shop_amount = 0;
	deal_amount = 0;
	
}
void ChangeShowMode(gboolean expand_all)		/*修改输出模式*/
{
	expand_all_info = expand_all;
}
void ShowDealOfSite(SiteInfo *site_current)		/*输出某个网站下面的所有交易信息*/
{
	gchar deal_data[DEAL_COLUMN][20];
	gchar *deal_data_pointer[DEAL_COLUMN];
	gint index;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	
	if (site_current == NULL)
		return ;
	for (index = 0; index < DEAL_COLUMN; index++)
		deal_data_pointer[index] = deal_data[index];
		
	gtk_clist_clear(GTK_CLIST(clist[2]));
	gtk_clist_freeze(GTK_CLIST(clist[2]));
	
	shop_current = site_current -> shop_head;
	while (shop_current != NULL)
	{
		deal_current = shop_current -> deal_head;
		while (deal_current != NULL)
		{
			sprintf(deal_data[0], "%d", deal_current -> shop_id);
			sprintf(deal_data[1], "%d", deal_current -> deal_id);
			sprintf(deal_data[2], "%d", deal_current -> pay_type);
			sprintf(deal_data[3], "%.2f", deal_current -> deal_money);
			strcpy(deal_data[4], deal_current -> deal_date);
			strcpy(deal_data[5], deal_current -> customer_addr);
			
			gtk_clist_append(GTK_CLIST(clist[2]), deal_data_pointer);
			deal_current = deal_current -> deal_next;
		}
		shop_current = shop_current -> shop_next;
	}
	gtk_clist_thaw(GTK_CLIST(clist[2]));
	
	//SortCList(2);	
	gtk_clist_moveto(GTK_CLIST(clist[2]), 0, 0, 0.0, 0.0);
	
	
}
void RealCheckData(void)		/*数据排错：更新所有编号，以及网站下店铺个数，店铺下交易次数*/
{
	g_message("CheckData");
	SiteInfo *site_current = site_head;
	ShopInfo *shop_current;
	DealInfo *deal_current;
	gint site_count = 0, shop_count = 0, deal_count = 0;
	gint shop_amount_one_site, deal_amount_one_shop;
	
	while (site_current != NULL)
	{	
		site_count++;
		site_current -> site_id = site_count;
			
		shop_current = site_current -> shop_head;
		shop_amount_one_site = 0;
		while (shop_current != NULL)
		{
			shop_count++;
			shop_current -> shop_id = shop_count;
			shop_current -> site_id = site_count;
			
			deal_current = shop_current -> deal_head;
			deal_amount_one_shop = 0;
			while (deal_current != NULL)
			{
				deal_count++;
				deal_current -> deal_id = deal_count;
				deal_current -> shop_id = shop_count;
				deal_amount_one_shop++;
				deal_current = deal_current -> deal_next;
			}			
			
			shop_amount_one_site++;
			shop_current -> deal_amount = deal_amount_one_shop;
			shop_current = shop_current -> shop_next;
		}
		site_current -> shop_amount = shop_amount_one_site;
		site_current = site_current -> site_next;
	}	
	
	site_amount = site_count;
	shop_amount = shop_count;
	deal_amount = deal_count;
	
	RedrawAllCList();
}
