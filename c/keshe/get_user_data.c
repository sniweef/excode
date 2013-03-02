/*
 * 前后台交互模块
 *
 * 主要功能：绘制对话框.同时为后台的链表操作获取必要的信息,例如打开、
 *         保存文件时的文件完整路径,修改后的新数据,查找所根据的key等
 */
 
#include <gtk/gtk.h>
#include <libgnomeui/gnome-dialog.h>
#include <libgnomeui/gnome-messagebox.h>
#include <libgnomeui/gnome-file-entry.h>
#include <string.h>
#include <stdlib.h>
#include "create_window.h"
#include "handle_data.h"

#define SITE_PAGE 0
#define SHOP_PAGE 1
#define DEAL_PAGE 2


/*下面6个变量定义于create_window.c*/
extern GtkWidget *window;
extern GtkWidget *notebook;
extern GtkWidget *clist[4];
extern gint site_selected_row;			/*存储网站页面选中的行数*/
extern gint shop_selected_row;			/*存储店铺页面选中的行数*/
extern gint deal_selected_row;			/*存储交易页面选中的行数*/

extern gboolean expand_all_info;			/*定义于handle_data.c,标识是否展开所有信息*/

gchar filename[50] = "";					/*存储文件完整路径*/
static gboolean file_modifyed = FALSE;		/*标识数据是否改动*/

void FileNew(void);				/*选择"文件/新建"*/
void FileOpen(void);			/*选择"文件/打开"*/
void SaveFile(void);			/*创建保存对话框*/

/**************************************************************************************************/
/*******************************关闭或打开新的文件时检查原数据是否改动的相关函数***************************/
/************************************************************************************************/
GtkWidget *yesno_dialog;						/*提示是否保存的对话框*/
GtkWidget *ConfirmDialog(gpointer data);		/*创建提示是否保存的对话框*/

void OnYesClicked(GtkButton *button, gpointer data)		/*按下YES*/
{
	gtk_widget_destroy(yesno_dialog);
	if (!strcmp(filename, ""))		/*如果文件路径为空*/
		SaveFile();
	else
		RealSave(filename);			/*执行真正保存的操作，位于handle_data.c*/
		
	file_modifyed = FALSE;
	
	if (!strcmp(data, "new"))			
		FileNew();		/*如果之前是执行“文件/新建”操作才调用此函数*/
	else if (!strcmp(data, "open"))		
		FileOpen();		/*如果之前是执行“文件/打开”操作才调用此函数*/
}
void OnNoClicked(GtkButton *button, gpointer data)		/*按下NO*/
{
	gtk_widget_destroy(yesno_dialog);
	
	
	if (!strcmp(data, "exit"))
		gtk_main_quit();
		
	file_modifyed = FALSE;
		
	if (!strcmp(data, "new"))
		FileNew();
	else if (!strcmp(data, "open"))
		FileOpen();
}
void OnCancelClicked(GtkButton *button)					/*按下CANCEL*/
{
	gtk_widget_destroy(yesno_dialog);
}

GtkWidget *ConfirmDialog(gpointer data)					/*创建提示是否保存的对话框*/
{
	GtkWidget *dialog;
	GtkWidget *vbox;
	GtkWidget *label;
	GtkWidget *image;
	GtkWidget *hbox;
	GtkWidget *bbox;
	GtkWidget *button;
	GtkWidget *sep;
	gchar *title = "数据已被修改。是否现在保存？";

	/*对话框窗口*/
	dialog = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	g_signal_connect(GTK_OBJECT(dialog), "delete_event", G_CALLBACK(gtk_widget_destroy), dialog);
	gtk_window_set_modal(GTK_WINDOW(dialog), TRUE);
	gtk_window_set_title(GTK_WINDOW(dialog), "警告");

	vbox = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(dialog), vbox);
	hbox = gtk_hbox_new(FALSE, 0);
	gtk_box_pack_start(GTK_BOX(vbox), hbox, FALSE, FALSE, 5);	

	/*创建按钮*/
	label = gtk_label_new(title);
	image = gtk_image_new_from_stock(GTK_STOCK_DIALOG_WARNING, GTK_ICON_SIZE_DIALOG);
	gtk_box_pack_start(GTK_BOX(hbox), image, FALSE, FALSE, 5);
	gtk_box_pack_start(GTK_BOX(hbox), label, FALSE, FALSE, 5);

	/*创建分隔符*/
	sep = gtk_hseparator_new();
	gtk_box_pack_start(GTK_BOX(vbox), sep, FALSE, FALSE, 5);
	bbox = gtk_hbutton_box_new();
	gtk_box_pack_start(GTK_BOX(vbox), bbox, FALSE, FALSE, 5);

	/*创建按钮*/
	button = gtk_button_new_from_stock(GTK_STOCK_YES);
	g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(OnYesClicked), data);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 5);
	button = gtk_button_new_from_stock(GTK_STOCK_NO);
	g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(OnNoClicked), data);
	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 5);
	button = gtk_button_new_from_stock(GTK_STOCK_CANCEL);
	g_signal_connect(GTK_OBJECT(button), "clicked", G_CALLBACK(OnCancelClicked), NULL);

	gtk_box_pack_start(GTK_BOX(bbox), button, FALSE, FALSE, 5);
	gtk_widget_show_all(dialog);
	return dialog;
}

gboolean OnWindowDeleteEvent(void)		/*退出前调用的函数*/
{
	if (file_modifyed == TRUE)
	{
		yesno_dialog = ConfirmDialog("exit");
		gtk_widget_show(yesno_dialog);
		return TRUE;
	}
	else
	{
		gtk_main_quit();		/*退出整个程序*/
		return FALSE;
	}
}	


/**************************************************************************************************/
/***************************************文件菜单栏需要调用的函数***************************************/
/*************************************************************************************************/
void InitCList(void);	/*除去统计页面并将当前页面切换到"网站"页面*/
void OnDataSaved(void);			/*数据已经保存*/

void StoreFilename(GtkFileSelection *selector, gpointer user_data);		/*保存文件路径*/

void FileNew(void)				/*选择"文件/新建"*/
{
	gint index;
	if (file_modifyed)
		yesno_dialog = ConfirmDialog("new");
	else
	{		
		for (index = 0; index < 3; index++)
			gtk_clist_clear(GTK_CLIST(clist[index]));
		
		InitCList();		
		strcpy(filename, "");
		ClearList();	/*将链表置为空*/
	}
}
void FileOpen(void)				/*选择"文件/打开"*/
{
	GtkWidget *open_dialog = NULL;
	
	if (file_modifyed)
		yesno_dialog = ConfirmDialog("open");
	else
	{
		open_dialog = gtk_file_selection_new("请选择一个文件:");
		g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(open_dialog)->ok_button), "clicked", G_CALLBACK(StoreFilename), open_dialog);
		g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(open_dialog)->ok_button), "clicked", G_CALLBACK(RealOpen), filename);
		g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(open_dialog)->ok_button), "clicked", G_CALLBACK(InitCList), NULL);
		g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(open_dialog)->ok_button), "clicked", G_CALLBACK(gtk_widget_destroy), open_dialog);
		g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(open_dialog)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer)open_dialog);
		
		gtk_widget_show(open_dialog);
	}
}
void FileSave(void)				/*选择"文件/保存"*/
{
	if (!strcmp(filename, ""))
		SaveFile();
	else 
	{
		RealSave(filename);
		OnDataSaved();
	}
}
void FileSaveas(void)			/*选择"文件/另存为"*/
{
	SaveFile();
}	
void ExpandAll(void)			/*选择"文件/更多/展开全部"*/
{
	ChangeShowMode(TRUE);			/*改变展示方式，TRUE"展开全部“模式，位于handle_data.c*/
	RedrawShopCList(0);
	RedrawDealCList(0, 0);
}
void RetractOthers(gint current_page)		/*选择"文件/更多/收起其他"*/
{
	gchar *str_site_id, *str_shop_id;
	gint site_id, shop_id;
	ShopInfo *shop_current;	
	
	switch (current_page)
	{
		case SITE_PAGE:
			ChangeShowMode(FALSE);	/*改变展示方式，FALSE为"收起其他模式"*/
			break;
		case SHOP_PAGE:
			if (shop_selected_row >= 0)
			{
				ChangeShowMode(FALSE);	
				gtk_clist_get_text(GTK_CLIST(clist[1]), shop_selected_row, 0, &str_site_id);
				site_id = atoi(str_site_id);
				RedrawShopCList(site_id);
			}
			else
				ShowMessageBox("请先选择某个商家！");
				
			break;
		case DEAL_PAGE:
			if (deal_selected_row >= 0)
			{
				ChangeShowMode(FALSE);
				gtk_clist_get_text(GTK_CLIST(clist[2]), deal_selected_row, 0, &str_shop_id);
				shop_id = atoi(str_shop_id);
				shop_current = GetShop(shop_id);
				site_id = shop_current -> site_id;
			
				RedrawDealCList(site_id, shop_id);
				RedrawShopCList(site_id);
			}
			else
				ShowMessageBox("请先选择某次交易！");
				
			break;
		default:
			ChangeShowMode(FALSE);
			break;
	}
}
void CheckData(void)			/*选择"文件/更多/数据排错"*/
{
	RealCheckData();
}
gboolean FileExit(void)			/*选择"文件/退出"*/
{
	if (file_modifyed == TRUE)
	{
		yesno_dialog = ConfirmDialog("exit");
		gtk_widget_show(yesno_dialog);
		return TRUE;
	}
	else
	{
		gtk_main_quit();		/*退出整个程序*/
		return FALSE;
	}
}
void OnDataChanged(void)		/*数据发生改变*/
{
	file_modifyed = TRUE;
}
void OnDataSaved(void)			/*数据已经保存*/
{
	file_modifyed = FALSE;
}
void StoreFilename(GtkFileSelection *selector, gpointer user_data)	/*获取文件名*/
{
	strcpy(filename, gtk_file_selection_get_filename(GTK_FILE_SELECTION(user_data)));
}
void SaveFile(void)				/*保存文件对话框*/
{	
	GtkWidget *save_dialog = NULL;
	
	save_dialog = gtk_file_selection_new("请输入文件名:");
	g_signal_connect(GTK_OBJECT(GTK_FILE_SELECTION(save_dialog)->ok_button), "clicked", G_CALLBACK(StoreFilename), save_dialog);
	g_message("SaveFile:%s", filename);
	g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(save_dialog)->ok_button), "clicked", G_CALLBACK(RealSave), filename);
	g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(save_dialog)->ok_button), "clicked", G_CALLBACK(OnDataSaved), NULL);
	g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(save_dialog)->ok_button), "clicked", G_CALLBACK(gtk_widget_destroy), save_dialog);
	g_signal_connect_swapped(GTK_OBJECT(GTK_FILE_SELECTION(save_dialog)->cancel_button), "clicked", G_CALLBACK(gtk_widget_destroy), (gpointer)save_dialog);
		
	gtk_widget_show(save_dialog);
}
void InitCList(void)	/*除去统计页面并将当前页面切换到"网站"页面*/
{
	g_message("InitCList");
	gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), 3);		
	gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), 0);
}

/**************************************************************************************************/
/******************************************编辑菜单栏需要调用的函数************************************/
/*************************************************************************************************/
#define SITE_COLUMN 4
#define SHOP_COLUMN 7
#define DEAL_COLUMN 6

#define ADD_SITE 0
#define EDIT_SITE 1
#define ADD_SHOP 0
#define EDIT_SHOP 1
#define ADD_DEAL 0
#define EDIT_DEAL 1

#define BUTTON_ADD 0
#define BUTTON_OK 1
#define BUTTON_CANCEL 2

/*以下两个变量定义于handle_data.c*/
extern gint site_amount;		/*网站总数*/
extern gint shop_amount;		/*店铺总数*/

/*存储选中行的数据*/
gchar site_data[4][50] = {"", "", "", ""};
gchar shop_data[7][20] = {"", "", "", "", "", ""};
gchar deal_data[6][20] = {"", "", "", "", "", ""};

/*上面三个数组对应的指针*/
gchar *site_data_pointer[4];	
gchar *shop_data_pointer[7];
gchar *deal_data_pointer[6];

gchar search_keys[3][50] = {"", "", ""};	/*存储搜索功能的关键字*/

void CreateSiteDialog(gint id, gint selected_row);		/*创建添加或编辑网站对话框*/
void CreateShopDialog(gint id, gint selected_row);		/*创建添加或编辑店铺对话框*/
void CreateDealDialog(gint id, gint selected_row);		/*创建添加或编辑交易对话框*/
gboolean SearchDialog(gint current_page);				/*创建查找对话框*/
gboolean DelSiteDialog(void);							/*创建删除网站对话框*/
gboolean DelShopDialog(void);							/*创建删除店铺对话框*/
gboolean DelDealDialog(void);							/*创建删除网站对话框*/
void CopyData(gint current_page);						/*拷贝所需的一些系统自动生成的数据(如商家编号，店铺总数)，用于编辑信息功能*/
gboolean SelectSearchedKey(gint current_page);			/*选取要根据哪些关键字进行搜索*/
gboolean SelectSearchedKind(gint current_page, gint search_key_id, gchar *search_key);		/*选取搜索哪类信息*/

void AddInfo(gint current_page)				/*选择"编辑/添加信息"*/
{
	g_message("AddInfo: current_page=%d", current_page);
	
	switch (current_page)	/*根据当前页面调用不同函数*/
	{
		case SITE_PAGE:
			CreateSiteDialog(ADD_SITE, 0);
			break;
		case SHOP_PAGE:
			CreateShopDialog(ADD_SHOP, 0);
			break;
		case DEAL_PAGE:
			CreateDealDialog(ADD_DEAL, 0);				
			break;
		default:
			break;
	}
	
}
void SearchInfo(gint current_page)					/*选择"编辑/查找信息"*/
{
	g_message("SearchInfo: current_page=%d", current_page);
	
	if (SearchDialog(current_page))
		SelectSearchedKey(current_page);
		
	
}
void EditInfo(gint current_page, gint selected_row)	/*选择"编辑/编辑信息"*/
{
	g_message("EditInfo: current_page=%d, selected_row=%d", current_page, selected_row);
	gint index;
	
	switch (current_page)		/*根据当前页面调用不同函数*/
	{
		case SITE_PAGE:
			/*获取选中行的数据*/
			for (index = 0; index < SITE_COLUMN; index++)
				gtk_clist_get_text(GTK_CLIST(clist[0]), selected_row, index, &site_data_pointer[index]);
			CopyData(SITE_PAGE);			
			CreateSiteDialog(EDIT_SITE, selected_row);				
			break;
		case SHOP_PAGE:
			for (index = 0; index < SHOP_COLUMN; index++)
				gtk_clist_get_text(GTK_CLIST(clist[1]), selected_row, index, &shop_data_pointer[index]);
			CopyData(SHOP_PAGE);
			CreateShopDialog(EDIT_SHOP, selected_row);
			break;
		case DEAL_PAGE:
			for (index = 0; index < DEAL_COLUMN; index++)
				gtk_clist_get_text(GTK_CLIST(clist[2]), selected_row, index, &deal_data_pointer[index]);
			CopyData(DEAL_PAGE);
			CreateDealDialog(EDIT_DEAL, selected_row);
			break;
	}
}
void DelInfo(gint current_page, gint selected_row)	/*选择"编辑/删除信息"*/		
{
	g_message("DelInfo: current_page=%d, selected_row=%d", current_page, selected_row);
	gint index;
	
	switch (current_page)	/*根据当前页面调用不同函数*/
	{
		case SITE_PAGE:
			for (index = 0; index < SITE_COLUMN; index++)
				gtk_clist_get_text(GTK_CLIST(clist[0]), selected_row, index, &site_data_pointer[index]);
			
			strcpy(site_data[0], site_data_pointer[0]);
			
			if (DelSiteDialog())
				DelSite(selected_row, site_data[0]);
			break;
		case SHOP_PAGE:
			for (index = 0; index < SHOP_COLUMN; index++)
				gtk_clist_get_text(GTK_CLIST(clist[1]), selected_row, index, &shop_data_pointer[index]);
				
			strcpy(shop_data[0], shop_data_pointer[0]);
			strcpy(shop_data[1], shop_data_pointer[1]);
			
			if (DelShopDialog())
				DelShop(selected_row, shop_data[0], shop_data[1], TRUE);
			break;
		case DEAL_PAGE:
			for (index = 0; index < DEAL_COLUMN; index++)
				gtk_clist_get_text(GTK_CLIST(clist[2]), selected_row, index, &deal_data_pointer[index]);
			
			strcpy(deal_data[0], deal_data_pointer[0]);
			strcpy(deal_data[1], deal_data_pointer[1]);
			
			if (DelDealDialog())
				DelDeal(selected_row, deal_data[0], deal_data[1], TRUE);
			break;
	}
}

void CreateSiteDialog(gint id, gint selected_row)	/*创建添加或编辑网站对话框*/	
{
	GtkWidget *site_dialog = NULL;
	GtkWidget *entry[2];
	GtkWidget *label;
	GtkWidget *table;
	GtkWidget *warning_label = NULL;
	gchar *dialog_title[2] = {"添加网站", "编辑网站"};
	gint index;
	gint reply;
	gboolean continue_add = TRUE;
	
	if (site_dialog != NULL)
	{
		g_message("site_dialog!=NULL");
		gdk_window_show(site_dialog->window);
		gdk_window_raise(site_dialog->window);
	}
	else
	{
		if (id == ADD_SITE)
			site_dialog = gnome_dialog_new(dialog_title[id], "添加", "确定","取消", NULL);
		else
			site_dialog = gnome_dialog_new(dialog_title[id], "确定","取消", NULL);
		table = gtk_table_new(3, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(site_dialog)->vbox),table, TRUE, TRUE, 0);
		
		/*以下均在绘制对话框*/
		label = gtk_label_new("网站名称 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[0] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[0], 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("网站地址 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[1] = gtk_entry_new_with_max_length(50);
		gtk_table_attach(GTK_TABLE(table), entry[1], 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		if (id == EDIT_SITE)	/*如果是编辑对话框，则给entry赋值*/
		{
			gtk_entry_set_text(GTK_ENTRY(entry[0]), site_data_pointer[1]);
			gtk_entry_set_text(GTK_ENTRY(entry[1]), site_data_pointer[2]);
		}
		
		/*修饰对话框，如设置按下ENTER键的默认操作等*/
		gnome_dialog_set_parent(GNOME_DIALOG(site_dialog), GTK_WINDOW(window));
		
		for (index = 0; index < 2; index++)
			gnome_dialog_editable_enters(GNOME_DIALOG(site_dialog), GTK_EDITABLE(entry[index]));
		if (id == ADD_SITE)
			gnome_dialog_set_default(GNOME_DIALOG(site_dialog), BUTTON_OK);
		else
			gnome_dialog_set_default(GNOME_DIALOG(site_dialog), BUTTON_OK - 1);
			
		gtk_widget_show_all(site_dialog);
	}
	
	while (continue_add)
	{
		reply = gnome_dialog_run(GNOME_DIALOG(site_dialog));
		if (id == EDIT_SITE && reply >= 0)
			reply++;	
			
		if (reply == BUTTON_CANCEL)		/*按下取消键*/
		{
			gtk_widget_destroy(site_dialog);
			continue_add = FALSE;
		}
		else if (reply == BUTTON_OK)	/*按下确定键*/
			 {
				 strcpy((gchar *)site_data[1], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
				 strcpy((gchar *)site_data[2], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[1])));
				 				 
				 if (strcmp(site_data[1], "") && strcmp(site_data[2], ""))		/*如果输入信息是完整的*/
				 {
					 if (id == ADD_SITE)
					 	AddSite(site_data);
					 else 
					 	EditSite(selected_row, site_data);
					 OnDataChanged();
					 continue_add = FALSE;
					 gtk_widget_destroy(site_dialog);
				 }	
				 else		/*如果输入信息不完整*/
				 {
				 	if (warning_label == NULL)
				 	{
				 		warning_label = gtk_label_new("请输入完整信息！");
				 		gtk_table_attach(GTK_TABLE(table), warning_label, 0, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
				 		gtk_widget_show(warning_label);
				 	}
				 	continue_add = TRUE;
				 }
				
			 }	
			 else if (reply == BUTTON_ADD)	/*按下添加键*/
			 {
			 	 strcpy((gchar *)site_data[1], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
				 strcpy((gchar *)site_data[2], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[1])));
				 g_message("网站名称: %s", (gchar *)site_data[1]);
				 g_message("网站地址: %s", (gchar *)site_data[2]);				 
				 
				 if (strcmp(site_data[1], "") && strcmp(site_data[2], ""))
				 {
				 	AddSite(site_data);
				 	OnDataChanged();
				 	if (warning_label != NULL)
				 	{
				 		gtk_widget_destroy(warning_label);
				 		warning_label = NULL;
				 	}
				 	gtk_entry_set_text(GTK_ENTRY(entry[0]), "");
				 	gtk_entry_set_text(GTK_ENTRY(entry[1]), "");
				 }
				 else
				 {
				 	if (warning_label == NULL)
				 	{
				 		warning_label = gtk_label_new("请输入完整信息！");
				 		gtk_table_attach(GTK_TABLE(table), warning_label, 0, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
				 		gtk_widget_show(warning_label);
				 	}
				 }
				 	
				 continue_add = TRUE;
			 } 
			 else
			 	 continue_add = FALSE;	
		if (id == EDIT_SITE)
			continue_add = FALSE;		
	}			
}
void CreateShopDialog(gint id, gint selected_row)	/*创建添加或编辑店铺对话框,类似于CreateSiteDialog，*/	
{
	g_message("CreateShopDialog");	
	GtkWidget *shop_dialog = NULL;
	GtkWidget *entry[5];
	GtkWidget *label;
	GtkWidget *table;
	GtkWidget *warning_label = NULL;
	gchar *dialog_title[2] = {"添加店铺", "编辑店铺"};
	gint index, site_id, reply;
	gboolean continue_add = TRUE;
	gchar former_site_id[10];
	
	if (shop_dialog != NULL)
	{
		g_message("shop_dialog!=NULL");
		gdk_window_show(shop_dialog->window);
		gdk_window_raise(shop_dialog->window);
	}
	else
	{
		/*绘制对话框*/
		if (id == ADD_SHOP)
			shop_dialog = gnome_dialog_new(dialog_title[id], "添加", "确定","取消", NULL);
		else
			shop_dialog = gnome_dialog_new(dialog_title[id], "确定","取消", NULL);
		table = gtk_table_new(5, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(shop_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("所属网站编号 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[0] = gtk_entry_new_with_max_length(10);			
		gtk_table_attach(GTK_TABLE(table), entry[0], 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("名称 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[1] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[1], 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("负责人 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[2] = gtk_entry_new_with_max_length(10);
		gtk_table_attach(GTK_TABLE(table), entry[2], 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("联系地 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[3] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[3], 1, 2, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("开户银行 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[4] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[4], 1, 2, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		
		if (id == EDIT_SHOP)
		{			
			gtk_entry_set_text(GTK_ENTRY(entry[0]), shop_data_pointer[0]);
			for (index = 1; index < 5; index++)
				gtk_entry_set_text(GTK_ENTRY(entry[index]), shop_data_pointer[index+1]);			
		}
		gnome_dialog_set_parent(GNOME_DIALOG(shop_dialog), GTK_WINDOW(window));
		
		for (index = 0; index < 5; index++)
			gnome_dialog_editable_enters(GNOME_DIALOG(shop_dialog), GTK_EDITABLE(entry[index]));
		if (id == ADD_SHOP)
			gnome_dialog_set_default(GNOME_DIALOG(shop_dialog), BUTTON_OK);
		else
			gnome_dialog_set_default(GNOME_DIALOG(shop_dialog), BUTTON_OK - 1);
		
		gtk_widget_show_all(shop_dialog);
	}	
	
	while (continue_add)
	{
		reply = gnome_dialog_run(GNOME_DIALOG(shop_dialog));
		if (id == EDIT_SHOP && reply >= 0)
			reply++;
		
		if (reply == BUTTON_CANCEL)			/*按下取消键*/
		{
			gtk_widget_destroy(shop_dialog);			
			continue_add = FALSE;
		}
		else if (reply == BUTTON_OK)		/*按下确定键*/
			 {
				 strcpy(shop_data[0], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
				 for (index = 2; index < 6; index++)
				 	strcpy(shop_data[index], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[index-1])));
				 	
				 site_id = atoi(shop_data[0]);
				 if (site_id > site_amount || site_id <= 0)
				 {
				 	if (warning_label == NULL)
				 	{	
				 		warning_label = gtk_label_new("无此网站,请检查所属网站编号!");
					 	gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(shop_dialog)->vbox),warning_label, TRUE, TRUE, 0);
					 	gtk_widget_show(warning_label);
				 	}
				 	continue_add = TRUE;
				 }
				 else
				 {					 	 
					 if (id == ADD_SHOP)
				 	 	AddShop(shop_data);
				 	 else
					 {
						g_message("shop_data_pointer[0] = %s", shop_data_pointer[0]);				 	 					 
					 	strcpy(former_site_id, shop_data_pointer[0]);
				 	 	EditShop(selected_row, former_site_id, shop_data);				 	 
				 	 }
				 	 OnDataChanged();
					 gtk_widget_destroy(shop_dialog);
					 continue_add =  FALSE;
				 }
			 }
			 else if (reply == BUTTON_ADD)	/*按下添加键*/
			 {
			 	 strcpy(shop_data[0], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
				 for (index = 2; index < 6; index++)
				 	strcpy(shop_data[index], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[index-1])));
				 	
				 site_id = atoi(shop_data[0]);
				 g_message("site_id=%d,site_amount=%d", site_id, site_amount);
				 if (site_id > site_amount || site_id <= 0)
				 {
				 	g_message("无此网站");
				 	if (warning_label == NULL)
				 	{
				 		g_message("warning_label == NULL");
				 		warning_label = gtk_label_new("无此网站,请检查所属网站编号!");
				 		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(shop_dialog)->vbox),warning_label, TRUE, TRUE, 0);
				 		gtk_widget_show(warning_label);	
				 	}				 				 	
				 }
				 else 
				 {
				 	if (warning_label != NULL)
				 	{
				 		gtk_widget_destroy(warning_label);	
				 		warning_label = NULL;
				 	}
				 	for (index = 1; index < 5; index++)
						gtk_entry_set_text(GTK_ENTRY(entry[index]), "");				
				 	AddShop(shop_data);
				 	OnDataChanged();
				 }
				 
				 continue_add = TRUE;
			 }
			 else
			 	 continue_add = FALSE;	
		if (id == EDIT_SHOP)
			continue_add = FALSE;		
	}
	
}

void CreateDealDialog(gint id, gint selected_row)		/*创建添加或编辑交易对话框*/		
{
	GtkWidget *deal_dialog = NULL;
	GtkWidget *entry[5];
	GtkWidget *label;
	GtkWidget *warning_label = NULL;
	GtkWidget *table;	
	gchar *dialog_title[2] = {"添加交易", "编辑交易"};
	gchar former_shop_id[10];
	gint index;
	gboolean continue_add = TRUE;
	gint reply;
	gint shop_id;
	
	if (deal_dialog != NULL)
	{
		g_message("deal_dialog!=NULL");
		gdk_window_show(deal_dialog->window);
		gdk_window_raise(deal_dialog->window);
	}
	else
	{
		/*绘制对话框*/
		if (id == ADD_DEAL)
			deal_dialog = gnome_dialog_new(dialog_title[id], "添加", "确定", "取消", NULL);
		else
			deal_dialog = gnome_dialog_new(dialog_title[id],  "确定", "取消", NULL);
		table = gtk_table_new(5, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(deal_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("所属店铺编号 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[0] = gtk_entry_new_with_max_length(10);			
		gtk_table_attach(GTK_TABLE(table), entry[0], 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("支付类型 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[1] = gtk_entry_new_with_max_length(1);
		gtk_table_attach(GTK_TABLE(table), entry[1], 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("金额 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[2] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[2], 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("日期 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[3] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[3], 1, 2, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		
		label = gtk_label_new("客户所在地 ");
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[4] = gtk_entry_new_with_max_length(20);
		gtk_table_attach(GTK_TABLE(table), entry[4], 1, 2, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		

		if (id == EDIT_DEAL)
		{	
			gtk_entry_set_text(GTK_ENTRY(entry[0]), deal_data_pointer[0]);		
			for (index = 1; index < 5; index++)
				gtk_entry_set_text(GTK_ENTRY(entry[index]), deal_data_pointer[index+1]);
		}
		
		gnome_dialog_set_parent((GnomeDialog *)deal_dialog, (GtkWindow *)window);
		
		for (index = 0; index < 5; index++)
			gnome_dialog_editable_enters(GNOME_DIALOG(deal_dialog), GTK_EDITABLE(entry[index]));
		if (id == ADD_DEAL)
			gnome_dialog_set_default((GnomeDialog *)deal_dialog, BUTTON_OK);
		else
			gnome_dialog_set_default((GnomeDialog *)deal_dialog, BUTTON_OK - 1);
			
		gtk_widget_show_all(deal_dialog);
	}
	
	while (continue_add)
	{
		reply = gnome_dialog_run(GNOME_DIALOG(deal_dialog));
		if (id == EDIT_DEAL && reply >= 0)
			reply++;
			
		if (reply == BUTTON_CANCEL)		/*按下取消键*/
		{
			gtk_widget_destroy(deal_dialog);
			continue_add = FALSE;
		}
		else if (reply == BUTTON_OK)	/*按下确定键*/
			 {
				 strcpy(deal_data[0], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
				 for (index = 2; index < 6; index++)
				 	strcpy(deal_data[index], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[index-1])));
				 
				 shop_id = atoi(deal_data[0]);	
				 if (shop_id > shop_amount)		/*如果shop_id大于shop_amount，即不存在该店铺编号*/
				 {
				 	if (warning_label == NULL)
				 	{
				 	 	warning_label = gtk_label_new("无此店铺,请检查所属店铺编号!");
				 		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(deal_dialog)->vbox),warning_label, TRUE, TRUE, 0);
				 		gtk_widget_show(warning_label);	
				 	}
				 	continue_add = TRUE;
				 }
				 else
				 {	
				 	 if (id == ADD_DEAL)
				 	 	AddDeal(deal_data);	
				 	 else
				 	 {			 	 	
				 	 	strcpy(former_shop_id, deal_data_pointer[0]);
				 	 	g_message("former_shop_id = %s", former_shop_id);
				 	 	EditDeal(selected_row, former_shop_id, deal_data);	 
				 	 }
				 	 OnDataChanged();
					 gtk_widget_destroy(deal_dialog);					
					 continue_add = FALSE;
				 }
			 } 
			 else if (reply == BUTTON_ADD)	/*按下添加键*/
			 {
			 	 strcpy(deal_data[0], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
				 for (index = 2; index < 6; index++)
				 	strcpy(deal_data[index], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[index-1])));
				 	
				 shop_id = atoi(deal_data[0]);
				 if (shop_id > shop_amount)
				 {
				 	if (warning_label == NULL)
				 	{
					 	warning_label = gtk_label_new("无此店铺,请检查所属店铺编号!");
					 	gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(deal_dialog)->vbox),warning_label, TRUE, TRUE, 0);
					 	gtk_widget_show(warning_label);					 	
					}
				 }
				 else
				 {	
				 	 if (warning_label != NULL)
				 	 {
				 	 	gtk_widget_destroy(warning_label);				 	
				 	 	warning_label = NULL;
				 	 }
				 	 AddDeal(deal_data);
				 	 OnDataChanged();	
				 	 for (index = 1; index < 5; index++)
						gtk_entry_set_text(GTK_ENTRY(entry[index]), "");
				 }	
				 	
				 continue_add = TRUE;
			 }
			 else
			 	continue_add = FALSE;
		if (id == EDIT_DEAL)
			continue_add = FALSE;
	}
}

#undef BUTTON_OK
#undef BUTTON_CANCEL
#define BUTTON_OK 0
#define BUTTON_CANCEL 1

gboolean SearchDialog(gint current_page)	/*查找对话框*/	
{
	GtkWidget *search_dialog = NULL;
	GtkWidget *table;
	GtkWidget *entry[3];
	GtkWidget *label;
	gchar *dialog_title[3] = {"查找网站", "查找店铺", "查找交易"};
	gchar *warning = "如果填写多项,将只根据第一项进行查询";
	gchar *search_basis[][3] = {{"根据名称 :", "根据地址 :"}, {"根据名称 :", "根据负责人 :", "根据联系地 :"}, {"根据日期 :", "根据客户地 :"}};
	
	gint index;
	
	if (search_dialog != NULL)
	{
		g_message("deal_dialog!=NULL");
		gdk_window_show(search_dialog->window);
		gdk_window_raise(search_dialog->window);
	}
	else
	{
		search_dialog = gnome_dialog_new(dialog_title[current_page], GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		if (current_page == SHOP_PAGE)
			table = gtk_table_new(4, 2, FALSE);
		else 
			table = gtk_table_new(3, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(search_dialog)->vbox),table, TRUE, TRUE, 0);
		
		
		
		label = gtk_label_new(search_basis[current_page][0]);
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[0] = gtk_entry_new_with_max_length(20);			
		gtk_table_attach(GTK_TABLE(table), entry[0], 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new(search_basis[current_page][1]);
		gtk_table_attach(GTK_TABLE(table), label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		entry[1] = gtk_entry_new_with_max_length(20);			
		gtk_table_attach(GTK_TABLE(table), entry[1], 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		if (current_page == SHOP_PAGE)
		{
			label = gtk_label_new(search_basis[current_page][2]);
			gtk_table_attach(GTK_TABLE(table), label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
			entry[2] = gtk_entry_new_with_max_length(20);			
			gtk_table_attach(GTK_TABLE(table), entry[2], 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
			
			label = gtk_label_new(warning);
			gtk_table_attach(GTK_TABLE(table), label, 0, 2, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		}
		else
		{	
			label = gtk_label_new(warning);
			gtk_table_attach(GTK_TABLE(table), label, 0, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		}	
		
		gnome_dialog_set_parent((GnomeDialog *)search_dialog, (GtkWindow *)window);
		for (index = 0; index < 2; index++)
			gnome_dialog_editable_enters(GNOME_DIALOG(search_dialog), GTK_EDITABLE(entry[index]));
		if (current_page == SHOP_PAGE)
			gnome_dialog_editable_enters(GNOME_DIALOG(search_dialog), GTK_EDITABLE(entry[2]));
		gnome_dialog_set_default((GnomeDialog *)search_dialog, BUTTON_OK);
		
		gtk_widget_show_all(search_dialog);		
	}
	
	gint reply = gnome_dialog_run(GNOME_DIALOG(search_dialog));
	if (reply == BUTTON_CANCEL)	
	{
		gtk_widget_destroy(search_dialog);
		return FALSE;
	}
	else if (reply == BUTTON_OK)
		 {
			 for (index = 0; index < 2; index++)
			 	strcpy(search_keys[index], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[index])));
			 if (current_page == SHOP_PAGE)
			 	strcpy(search_keys[2], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[2])));
			 g_message("search_keys[0]:%s", search_keys[0]);
			 g_message("search_keys[1]:%s", search_keys[1]);
			 g_message("search_keys[2]:%s", search_keys[2]);
			 gtk_widget_destroy(search_dialog);
			 return TRUE;
		 }	
	return FALSE;
}


gboolean DelSiteDialog(void)	/*删除网站对话框*/
{
	GtkWidget *del_site_dialog = NULL;
	GtkWidget *label;
	GtkWidget *table;
	gchar *dialog_title = "删除网站";
	
	if (del_site_dialog != NULL)
	{
		g_message("del_site_dialog!=NULL");
		gdk_window_show(del_site_dialog->window);
		gdk_window_raise(del_site_dialog->window);
	}
	else
	{
		del_site_dialog = gnome_dialog_new(dialog_title, GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		table = gtk_table_new(2, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_site_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("网站名称 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(site_data_pointer[1]);
		gtk_table_attach((GtkTable*)table, label, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("网站地址 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(site_data_pointer[2]);
		gtk_table_attach((GtkTable*)table,label, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_site_dialog)->vbox),label, TRUE, TRUE, 0);
		label = gtk_label_new("你确认要删除此网站及其以下所有信息?");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_site_dialog)->vbox),label, TRUE, TRUE, 0);
		label = gtk_label_new("(将重新对店铺及交易信息进行编号)");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_site_dialog)->vbox),label, TRUE, TRUE, 0);
		
		gnome_dialog_set_parent((GnomeDialog *)del_site_dialog, (GtkWindow *)window);		
		
		gtk_widget_show_all(del_site_dialog);		
	}
	gint reply = gnome_dialog_run(GNOME_DIALOG(del_site_dialog));
	if (reply == BUTTON_CANCEL)
	{
		gtk_widget_destroy(del_site_dialog);
		return FALSE;
	}
	else if (reply == BUTTON_OK)
		 {
			 gtk_widget_destroy(del_site_dialog);
			 OnDataChanged();	
			 return TRUE;
		 }
	return FALSE;
}

gboolean DelShopDialog(void)		/*删除店铺对话框*/
{
	GtkWidget *del_shop_dialog = NULL;
	GtkWidget *label;
	GtkWidget *table;
	gchar *dialog_title = "删除店铺";
	
	if (del_shop_dialog != NULL)
	{
		g_message("del_shop_dialog!=NULL");
		gdk_window_show(del_shop_dialog->window);
		gdk_window_raise(del_shop_dialog->window);
	}
	else
	{
		del_shop_dialog = gnome_dialog_new(dialog_title, GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		table = gtk_table_new(5, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_shop_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("所属网站编号 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(shop_data_pointer[0]);			
		gtk_table_attach((GtkTable*)table, label, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("名称 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(shop_data_pointer[2]);
		gtk_table_attach((GtkTable*)table, label, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("负责人 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(shop_data_pointer[3]);
		gtk_table_attach((GtkTable*)table, label, 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("联系地 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(shop_data_pointer[4]);
		gtk_table_attach((GtkTable*)table, label, 1, 2, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("开户银行 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(shop_data_pointer[5]);
		gtk_table_attach((GtkTable*)table, label, 1, 2, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_shop_dialog)->vbox),label, TRUE, TRUE, 0);
		
		label = gtk_label_new("你确认要删除此店铺及其以下所有信息?");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_shop_dialog)->vbox),label, TRUE, TRUE, 0);
		
		label = gtk_label_new("(将重新对交易信息进行编号)");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_shop_dialog)->vbox),label, TRUE, TRUE, 0);
		
		gnome_dialog_set_parent((GnomeDialog *)del_shop_dialog, (GtkWindow *)window);
		
		gtk_widget_show_all(del_shop_dialog);
	}
	
	gint reply = gnome_dialog_run(GNOME_DIALOG(del_shop_dialog));
	if (reply == BUTTON_CANCEL)
	{
		gtk_widget_destroy(del_shop_dialog);
		return FALSE;
	}
	else if (reply == BUTTON_OK)
		 {
		 	  gtk_widget_destroy(del_shop_dialog);
		 	  OnDataChanged();	
		 	  return TRUE;
		 }	
	return FALSE;
}

gboolean DelDealDialog(void)		/*删除交易对话框*/	
{
	
	GtkWidget *del_deal_dialog = NULL;
	GtkWidget *label;
	GtkWidget *table;
	gchar *dialog_title = "删除交易";
	
	if (del_deal_dialog != NULL)
	{
		g_message("del_deal_dialog!=NULL");
		gdk_window_show(del_deal_dialog->window);
		gdk_window_raise(del_deal_dialog->window);
	}
	else
	{
		del_deal_dialog = gnome_dialog_new(dialog_title, GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		table = gtk_table_new(5, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_deal_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("所属店铺编号 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(deal_data_pointer[0]);			
		gtk_table_attach((GtkTable*)table, label, 1, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("支付类型 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(deal_data_pointer[2]);	
		gtk_table_attach((GtkTable*)table, label, 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
	
		label = gtk_label_new("金额 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(deal_data_pointer[3]);	
		gtk_table_attach((GtkTable*)table, label, 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("日期 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(deal_data_pointer[4]);	
		gtk_table_attach((GtkTable*)table, label, 1, 2, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("客户所在地 ");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);		
		label = gtk_label_new(deal_data_pointer[5]);	
		gtk_table_attach((GtkTable*)table, label, 1, 2, 4, 5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_deal_dialog)->vbox),label, TRUE, TRUE, 0);
		label = gtk_label_new("你确认要删除此次交易信息?");
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(del_deal_dialog)->vbox),label, TRUE, TRUE, 0);
		
		gnome_dialog_set_parent((GnomeDialog *)del_deal_dialog, (GtkWindow *)window);
		
		gtk_widget_show_all(del_deal_dialog);
	}
	
	gint reply = gnome_dialog_run(GNOME_DIALOG(del_deal_dialog));
	if (reply == BUTTON_CANCEL)
	{
		gtk_widget_destroy(del_deal_dialog);
		return FALSE;
	}
	else if (reply == BUTTON_OK)
		 {
			 gtk_widget_destroy(del_deal_dialog);
			 OnDataChanged();	
			 return TRUE;
		 }
	return FALSE;
}
void CopyData(gint current_page)			/*拷贝所需的一些系统自动生成的数据(如商家编号，店铺总数)，用于编辑信息功能*/
{
	switch (current_page)	
	{
		case SITE_PAGE:
			strcpy(site_data[0], site_data_pointer[0]);
			strcpy(site_data[3], site_data_pointer[3]);
			break;
		case SHOP_PAGE:
			strcpy(shop_data[1], shop_data_pointer[1]);
			strcpy(shop_data[6], shop_data_pointer[6]);
			break;
		case DEAL_PAGE:
			strcpy(deal_data[1], deal_data_pointer[1]);
			break;
	}
}
gboolean SelectSearchedKey(gint current_page)		/*选取搜索根据的关键字*/	
{
	g_message("SelectSearchedKey");	
	
	if (strcmp(search_keys[0], ""))		/*如果第一项不为空，根据第一项进行搜索，下面类似*/
		return SelectSearchedKind(current_page, 0, search_keys[0]);
	else if (strcmp(search_keys[1], ""))
		return SelectSearchedKind(current_page, 1, search_keys[1]);	
	else if (strcmp(search_keys[2], ""))	
		return SelectSearchedKind(current_page, 2, search_keys[2]);
	else
	{
		g_message("no input");
		ShowMessageBox("未输入查找项!");
		return FALSE;
	}
}
gboolean SelectSearchedKind(gint current_page, gint search_key_id, gchar *search_key)		/*选取搜索哪类信息*/
{
	switch (current_page)
	{
		case SITE_PAGE:
			g_message("search site");
			return SearchSite(search_key_id, search_key);
		case SHOP_PAGE:
			g_message("search shop");
			return SearchShop(search_key_id, search_key);			
		case DEAL_PAGE:
			g_message("search deal");
			return SearchDeal(search_key_id, search_key);
	}
	return FALSE;
}
/**************************************************************************************************/
/***************************************统计菜单栏需要调用的函数***************************************/
/*************************************************************************************************/
void StatiSiteDialog(void);		/*选择"统计/网站排行榜"*/
void StatiShopDialog(void);		/*选择"统计/店铺交易情况"*/
void StatiDealDialog(void);		/*选择"统计/地区贸易情况"*/

void StatiInfo(gint id)		/*根据id选择统计哪方面信息*/
{
	void (*stati_func_pointer[3])(void) = {StatiSiteDialog, StatiShopDialog, StatiDealDialog};
	
	if (site_amount == 0)
	{
		ShowMessageBox("无网站信息！");
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), SITE_PAGE);
	}
	else if (shop_amount == 0)
	{
		ShowMessageBox("无店铺信息！");
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), SHOP_PAGE);
	}
	else
		stati_func_pointer[id]();
}


void StatiSiteDialog(void)	/*选择"统计/网站排行榜"*/
{
	GtkWidget *stati_site_dialog = NULL;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *warning_label = NULL;
	gchar *dialog_title = "网站排行榜";
	gchar date[20];
	gint reply;
	
	if (stati_site_dialog != NULL)
	{
		g_message("stati_site_dialog!=NULL");
		gdk_window_show(stati_site_dialog->window);
		gdk_window_raise(stati_site_dialog->window);
	}
	else
	{
		stati_site_dialog = gnome_dialog_new(dialog_title, GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		table = gtk_table_new(3, 1, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(stati_site_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("请输入年月份(如201010)：");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
		entry = gtk_entry_new_with_max_length(20);
		gtk_table_attach((GtkTable*)table, entry, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		gnome_dialog_set_parent((GnomeDialog *)stati_site_dialog, (GtkWindow *)window);		
		gnome_dialog_editable_enters(GNOME_DIALOG(stati_site_dialog), GTK_EDITABLE(entry));
		gnome_dialog_set_default((GnomeDialog *)stati_site_dialog, BUTTON_OK);
		
		gtk_widget_show_all(stati_site_dialog);
	}
	
	while (1)
	{
		reply = gnome_dialog_run(GNOME_DIALOG(stati_site_dialog));
		if (reply == BUTTON_OK)
		{
			strcpy(date, (gchar *)gtk_entry_get_text(GTK_ENTRY(entry)));
			if (!strcmp(date, ""))
			{
				if (warning_label == NULL)
				{
					warning_label = gtk_label_new("请输入完整信息！");
					gtk_table_attach((GtkTable*)table, warning_label, 0, 1, 2,3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
					gtk_widget_show(warning_label);	
				}
				continue;	
			}
			gtk_widget_destroy(stati_site_dialog);
			StatiSite(date);		
			break;
		}
		else if (reply == BUTTON_CANCEL)
		{
			gtk_widget_destroy(stati_site_dialog);
			break;
		}
		else
			break;
	}
}
void StatiShopDialog(void)	/*选择"统计/店铺交易情况"*/
{
	GtkWidget *stati_shop_dialog = NULL;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *entry;
	GtkWidget *warning_label = NULL;
	gchar *dialog_title = "店铺交易信息";
	gchar date[20];
	gint reply;
	
	if (stati_shop_dialog != NULL)
	{
		g_message("stati_shop_dialog!=NULL");
		gdk_window_show(stati_shop_dialog->window);
		gdk_window_raise(stati_shop_dialog->window);
	}
	else
	{
		stati_shop_dialog = gnome_dialog_new(dialog_title, GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		table = gtk_table_new(3, 1, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(stati_shop_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("请输入年份(如2010)：");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
		entry = gtk_entry_new_with_max_length(20);
		gtk_table_attach((GtkTable*)table, entry, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		gnome_dialog_set_parent((GnomeDialog *)stati_shop_dialog, (GtkWindow *)window);		
		gnome_dialog_editable_enters(GNOME_DIALOG(stati_shop_dialog), GTK_EDITABLE(entry));
		gnome_dialog_set_default((GnomeDialog *)stati_shop_dialog, BUTTON_OK);
		
		gtk_widget_show_all(stati_shop_dialog);
	}
	
	while (1)
	{
		reply = gnome_dialog_run(GNOME_DIALOG(stati_shop_dialog));
		if (reply == BUTTON_OK)
		{
			strcpy(date, (gchar *)gtk_entry_get_text(GTK_ENTRY(entry)));
			if (!strcmp(date, ""))
			{
				if (warning_label == NULL)
				{
					warning_label = gtk_label_new("请输入完整信息！");
					gtk_table_attach((GtkTable*)table, warning_label, 0, 1, 2,3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
					gtk_widget_show(warning_label);	
				}
				continue;	
			}
		
			gtk_widget_destroy(stati_shop_dialog);
			StatiShop(date);	
			break;	
		}
		else if (reply == BUTTON_CANCEL)
		{
			gtk_widget_destroy(stati_shop_dialog);
			break;
		}
		else
			break;
	}
}
void StatiDealDialog(void)	/*选择"统计/地区贸易情况"*/
{
	GtkWidget *stati_deal_dialog = NULL;
	GtkWidget *table;
	GtkWidget *label;
	GtkWidget *entry[3];
	GtkWidget *warning_label = NULL;
	gchar *dialog_title = "地区贸易关系";
	gchar stati_data[3][20];
	gint reply, index;
	
	if (stati_deal_dialog != NULL)
	{
		g_message("stati_deal_dialog!=NULL");
		gdk_window_show(stati_deal_dialog->window);
		gdk_window_raise(stati_deal_dialog->window);
	}
	else
	{
		stati_deal_dialog = gnome_dialog_new(dialog_title, GNOME_STOCK_BUTTON_OK, GNOME_STOCK_BUTTON_CANCEL, NULL);
		table = gtk_table_new(5, 2, FALSE);
		gtk_box_pack_start(GTK_BOX(GNOME_DIALOG(stati_deal_dialog)->vbox),table, TRUE, TRUE, 0);
		
		label = gtk_label_new("请输入两地区名称和年月份：");
		gtk_table_attach((GtkTable*)table, label, 0, 2, 0, 1, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("地区一：");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
		entry[0] = gtk_entry_new_with_max_length(20);
		gtk_table_attach((GtkTable*)table, entry[0], 1, 2, 1, 2, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("地区二：");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
		entry[1] = gtk_entry_new_with_max_length(20);
		gtk_table_attach((GtkTable*)table, entry[1], 1, 2, 2, 3, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		label = gtk_label_new("年月份：");
		gtk_table_attach((GtkTable*)table, label, 0, 1, 3, 4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);	
		entry[2] = gtk_entry_new_with_max_length(20);
		gtk_table_attach((GtkTable*)table, entry[2], 1, 2, 3,4, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
		
		gnome_dialog_set_parent((GnomeDialog *)stati_deal_dialog, (GtkWindow *)window);		
		for (index = 0; index < 3; index++)
			gnome_dialog_editable_enters(GNOME_DIALOG(stati_deal_dialog), GTK_EDITABLE(entry[index]));
		gnome_dialog_set_default((GnomeDialog *)stati_deal_dialog, BUTTON_OK);
		
		gtk_widget_show_all(stati_deal_dialog);
	}
	
	while (1)
	{
		reply = gnome_dialog_run(GNOME_DIALOG(stati_deal_dialog));
		if (reply == BUTTON_OK)
		{
			strcpy(stati_data[0], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[0])));
			strcpy(stati_data[1], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[1])));
			strcpy(stati_data[2], (gchar *)gtk_entry_get_text(GTK_ENTRY(entry[2])));
		
			if (!strcmp(stati_data[0], "") || !strcmp(stati_data[1], "") || !strcmp(stati_data[2], ""))
			{
				if (warning_label == NULL)
				{
					warning_label = gtk_label_new("请输入完整信息！");
					gtk_table_attach((GtkTable*)table, warning_label, 0, 2, 4,5, GTK_EXPAND | GTK_FILL, GTK_EXPAND | GTK_FILL, 5, 5);
					gtk_widget_show(warning_label);					
				}	
				continue;		
			}
			g_message("area1=%s,area2=%s", (gchar *)stati_data[0], (gchar *)stati_data[1]);
			gtk_widget_destroy(stati_deal_dialog);
			StatiDeal(stati_data);
			break;
		}
		else  if (reply == BUTTON_CANCEL)
		{
			gtk_widget_destroy(stati_deal_dialog);
			break;
		}
		else
			break;
	}
}

/**********************************************************************************************************/
/*************************************************其他函数**************************************************/
/*********************************************************************************************************/
void HandleSwitchPage(gint current_page, gint former_page)		/*处理切换页面*/
{
	gchar *str_site_id, *str_shop_id;
	gint site_id = 0, shop_id = 0;
	SiteInfo *site_current;
	ShopInfo *shop_current;	
	
	if (!expand_all_info)	/*如果是“收起其他”模式*/
	{
		g_message("former_page=%d,current_page=%d", former_page, current_page); 
		if (current_page == SHOP_PAGE)	
		{
			if (former_page == SITE_PAGE)		/*从网站页面转到店铺页面*/
			{
				gtk_clist_get_text(GTK_CLIST(clist[0]), site_selected_row < 0 ? 0 : site_selected_row, 0, &str_site_id);
				site_id = atoi(str_site_id);
				RedrawShopCList(site_id);
			}
			else if (former_page == DEAL_PAGE)	/*从交易页面转到店铺页面*/
			{
				gtk_clist_get_text(GTK_CLIST(clist[2]), deal_selected_row < 0 ? 0 : deal_selected_row, 0, &str_shop_id);
				shop_id = atoi(str_shop_id);
				
				g_message("shop_id=%d", shop_id);
				shop_current = GetShop(shop_id);	/*GetShop:根据shop_id返回对应的shop节点，位于handle_data.c*/
				if (shop_current != NULL)
					site_id = shop_current -> site_id;
				else
				{
					gtk_clist_get_text(GTK_CLIST(clist[0]), site_selected_row < 0 ? 0 : site_selected_row, 0, &str_site_id);
					site_id = atoi(str_site_id);
				}
				RedrawShopCList(site_id);	/*RedrawShopCList:根据site_id刷新店铺信息下的clist，位于handle_data.c*/
			}
		}
		else if (current_page == DEAL_PAGE)
		{
			if (former_page == SITE_PAGE)			/*从网站页面切换到交易页面*/
			{
				gtk_clist_get_text(GTK_CLIST(clist[0]), site_selected_row < 0 ? 0 : site_selected_row, 0, &str_site_id);
				site_id = atoi(str_site_id);
				
				site_current = GetSite(site_id);	/*GetSite:根据site_id返回对应的site节点，位于handle_data.c*/
				ShowDealOfSite(site_current);		/*ShowDealOfSite:输出site_current所指网站的所有交易信息，位于handle_data.c*/
			}
			else if (former_page == SHOP_PAGE)		/*从店铺页面切换到交易页面*/
			{
				gtk_clist_get_text(GTK_CLIST(clist[1]), shop_selected_row < 0 ? 0 : shop_selected_row, 0, &str_site_id);
				gtk_clist_get_text(GTK_CLIST(clist[1]), shop_selected_row < 0 ? 0 : shop_selected_row, 1, &str_shop_id);
				site_id = atoi(str_site_id);
				shop_id = atoi(str_shop_id);			
				
				RedrawDealCList(site_id, shop_id);	/*RedrawDealCList:刷新交易信息下的clist，位于handle_data.c*/
			}
			
		}
	}
}

