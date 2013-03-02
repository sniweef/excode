/*
 * 主窗口绘制模块
 *
 * 主要功能:绘制主窗口,同时为主窗口中建立回呼函数并获取必要的信息.例如当前选中标签页,当前选中行数
 */

#include <gnome.h>
#include <gtk/gtk.h>
#include <stdlib.h>
#include <libgnomeui/gnome-about.h>
#include <libgnomeui/gnome-app-helper.h>
#include <libgnomeui/gnome-messagebox.h>
#include "get_user_data.h"

#define SITE_COLUMN 4
#define SHOP_COLUMN 7
#define DEAL_COLUMN 6
#define SITE_PAGE 0
#define SHOP_PAGE 1
#define DEAL_PAGE 2
#define STATI_PAGE 3


GtkWidget *window = NULL;
GtkWidget *notebook;
GtkWidget *clist[4];

gint site_selected_row = -1;			/*存储网站页面选中的行数*/
gint shop_selected_row = -1;			/*存储店铺页面选中的行数*/
gint deal_selected_row = -1;			/*存储交易页面选中的行数*/

void OnFileNewActivate(void);
void OnFileOpenActivate(void);			/*选择"文件/打开"*/
void OnFileSaveActivate(void);			/*选择"文件/保存"*/
void OnFileSaveasActivate(void);		/*选择"文件/另存为"*/
void OnExpandAllActivate(void);			/*选择"文件/更多/展开全部"*/
void OnRetractOthersActivate(void);		/*选择"文件/更多/收起其他"*/
void OnCheckDataActivate(void);			/*选择"文件/更多/数据排错"*/
void OnFileExitActivate(void);			/*选择"文件/退出"*/

void OnAddInfoActivate(void);			/*选择"编辑/添加信息"*/
void OnSearchInfoActivate(void);		/*选择"编辑/查找信息"*/
void OnEditInfoActivate(void);			/*选择"编辑/编辑信息"*/
void OnDelInfoActivate(void);			/*选择"编辑/删除信息"*/

void OnStatiSiteActivate(void);			/*选择"统计/网站排行榜"*/
void OnStatiShopActivate(void);			/*选择"统计/店铺交易情况"*/
void OnStatiDealActivate(void);			/*选择"统计/地区贸易关系"*/

void OnHelpActivate(void);				/*选择"帮助/操作说明"*/
void OnAboutActivate(void);				/*选择"帮助/关于本程序"*/

void ShowMessageBox(gchar *message);				/*弹出内容为message的信息框*/
void SetColumnWidth(GtkWidget *clist, gint id);		/*设置标签宽度*/
void SelectionMade( GtkWidget *clist, gint row, gint column, GdkEventButton *event,	gpointer data );		/*选中某一行调用的回呼函数*/
void UnselectionMade(GtkWidget *clist,gint row,gint column,	GdkEventButton *event, gpointer data);			/*取消选中某一行调用的回呼函数*/
gboolean PageSelected(GtkNotebook *notebook, gint arg, gpointer data) ;			/*选中某一页面调用的回响函数*/
void SortByColumn(GtkCList *clist, gint column);								/*根据某一列对整个CList进行排序*/



/**************************************************************************************************/
/******************************************创建主窗口的相关函数***************************************/
/*************************************************************************************************/
GtkItemFactoryEntry menu[] = {		/*菜单*/

		{"/文件(_D)",					"<alt>D",			0,							0,	"<Branch>"},
		{"/文件(_D)/新建(_N)",			"<ctrl>N",			OnFileNewActivate,			0},
		{"/文件(_D)/打开(_O)",			"<ctrl>O",			OnFileOpenActivate,			0},
		{"/文件(_D)/保存(_S)",			"<ctrl>S",			OnFileSaveActivate,			0},
		{"/文件(_D)/另存为(_S)",			"<ctrl><shift>S",	OnFileSaveasActivate,		0},
		{"/文件(_D)/更多",				NULL,	 			0,							0,	"<Branch>"},
		{"/文件(_D)/更多/展开全部(_E)",	"<ctrl><shift>E", 	OnExpandAllActivate,		0},
		{"/文件(_D)/更多/收起其他(_R)",	"<ctrl><shift>R", 	OnRetractOthersActivate,	0},
		{"/文件(_D)/更多/数据排错(_D)",	"<ctrl><shift>D", 	OnCheckDataActivate,		0},
		{"/文件(_D)/退出(_Q)",			"<ctrl>Q",			OnFileExitActivate,			0},

		{"/编辑(_E)",					"<alt>E",			0,							0,	"<Branch>"},
		{"/编辑(_E)/添加信息(_A)",			"<ctrl>A",			OnAddInfoActivate,			0},
		{"/编辑(_E)/查找信息(_F)",			"<ctrl>F",			OnSearchInfoActivate,		0},
		{"/编辑(_E)/修改信息(_E)",			"<ctrl>E",			OnEditInfoActivate,			0},
		{"/编辑(_E)/删除信息(_D)",			"<ctrl>D",			OnDelInfoActivate,			0},
	
		{"/统计(_S)",					"<alt>S",			0,							0,	"<Branch>"},
		{"/统计(_S)/网站排名榜(_B)",		"<ctrl>B",			OnStatiSiteActivate,		0},
		{"/统计(_S)/店铺交易情况(_C)",		"<ctrl>C",			OnStatiShopActivate,		0},
		{"/统计(_S)/地区贸易关系(_T)",		"<ctrl>T",			OnStatiDealActivate,		0},			
			
		{"/帮助(_H)",					"<alt>H",			0,							0,	"<Branch>"},
		{"/帮助(_H)/操作说明(_H)",			"<ctrl>H",			OnHelpActivate,				0},		
		{"/帮助(_H)/关于本程序(_A)",		"<ctrl><shift>A",	OnAboutActivate,			0}		
};	

/*创建主窗口*/
void CreateWindow(void)
{	
	GtkWidget *v_box;
	GtkWidget *scrolled_window;
	GtkWidget *label;
	GtkWidget *toolbar;
	GtkWidget *statusbar;
	GtkWidget *handlebox;
	GtkAccelGroup *accel_group;
	GtkItemFactory *item_factory;
	
	gint count;
	gint clist_column[3] = {SITE_COLUMN, SHOP_COLUMN, DEAL_COLUMN};			/*每个clist的列数*/
	gint nmenu_items = sizeof(menu) / sizeof(menu[0]);						/*菜单数目*/
	gchar *clist_titles[][7] ={ 
		{"网站编号", "名称", "地址", "店铺总数"},
		{"所属网站编号", "店铺编号", "名称", "负责人", "联系地", "开户银行", "交易次数"},
		{"所属店铺编号", "交易编号", "支付类型", "金额", "日期", "客户所在地"},
	};		/*每个clist的标签文字*/
	gchar *window_titles = "购物网站信息管理系统";
	gchar *label_titles[3] = {"网站信息",  "店铺信息", "交易信息"};	/*notebook构件每个页面的标签文字*/
		
	/*创建工具条*/
	static GnomeUIInfo gnome_toolbar[] = {
		GNOMEUIINFO_ITEM_STOCK (N_("新建"), N_("新建数据文件"), OnFileNewActivate, GNOME_STOCK_PIXMAP_NEW),
		GNOMEUIINFO_ITEM_STOCK (N_("打开"), N_("打开数据文件"), OnFileOpenActivate, GNOME_STOCK_PIXMAP_OPEN),
		GNOMEUIINFO_ITEM_STOCK (N_("保存"), N_("保存当前数据"), OnFileSaveActivate, GNOME_STOCK_PIXMAP_SAVE),
		GNOMEUIINFO_ITEM_STOCK (N_("另存为"), N_("保存数据到其他目录"), OnFileSaveasActivate, GNOME_STOCK_PIXMAP_SAVE_AS),
  		GNOMEUIINFO_SEPARATOR,
  		
 		GNOMEUIINFO_ITEM_STOCK (N_("添加"), N_("添加信息"), OnAddInfoActivate, GNOME_STOCK_PIXMAP_ADD),
  		GNOMEUIINFO_ITEM_STOCK (N_("查找"), N_("查找信息"), OnSearchInfoActivate, GNOME_STOCK_PIXMAP_SEARCH),
  		GNOMEUIINFO_ITEM_STOCK (N_("修改"), N_("修改信息"), OnEditInfoActivate, GNOME_STOCK_PIXMAP_SRCHRPL),
  		GNOMEUIINFO_ITEM_STOCK (N_("删除"), N_("删除信息"), OnDelInfoActivate, GNOME_STOCK_PIXMAP_REMOVE),
  		
  		GNOMEUIINFO_END
	};
	
	/*主窗口相关设置*/
	window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
	gtk_widget_set_usize(window, 900, 700);
	gtk_container_border_width(GTK_CONTAINER(window), 0);
	gtk_window_set_title(GTK_WINDOW(window), window_titles);

	/*建立回呼函数以处理信号*/
	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(gtk_widget_destroyed),
					&window);
	gtk_signal_connect(GTK_OBJECT(window), "delete_event", GTK_SIGNAL_FUNC(OnWindowDeleteEvent),
					NULL);
	
	/*建立菜单*/
	accel_group = gtk_accel_group_new();
	item_factory = gtk_item_factory_new(GTK_TYPE_MENU_BAR, "<blah>", accel_group);
	gtk_item_factory_create_items(item_factory, nmenu_items, menu, NULL);
	gtk_window_add_accel_group((GtkWindow *)GTK_OBJECT(window), accel_group);	
				
	/*将菜单包含在v_box里面*/
	v_box = gtk_vbox_new(FALSE, 0);
	gtk_container_add(GTK_CONTAINER(window), v_box);
	gtk_box_pack_start(GTK_BOX(v_box), gtk_item_factory_get_widget(item_factory, "<blah>"), FALSE, FALSE, 0);
	
	/*建立handle_box跟toolbar*/
	toolbar = gtk_toolbar_new();		
	gnome_app_fill_toolbar(GTK_TOOLBAR(toolbar), gnome_toolbar, NULL);
	handlebox = gtk_handle_box_new();
	gtk_container_add(GTK_CONTAINER(handlebox), toolbar);	
	gtk_box_pack_start(GTK_BOX(v_box), handlebox, FALSE, FALSE, 0);
	
	/*建立笔记本构件*/
	notebook = gtk_notebook_new();
	gtk_notebook_set_tab_pos(GTK_NOTEBOOK(notebook), GTK_POS_TOP);		
	gtk_box_pack_start(GTK_BOX(v_box), notebook, TRUE, TRUE, 0);
	
	for (count = 0; count < 3; count++)
	{
		/*建立滚动窗口*/
		scrolled_window = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window), GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
		label = gtk_label_new(label_titles[count]);
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolled_window, label);		
		gtk_container_set_border_width(GTK_CONTAINER(scrolled_window),5);
			
		/*建立GtkCList*/
		clist[count] = gtk_clist_new_with_titles(clist_column[count], clist_titles[count]);		
		/*设置边框阴影*/
		gtk_clist_set_shadow_type(GTK_CLIST(clist[count]), GTK_SHADOW_OUT);
		/*设置宽度*/
		SetColumnWidth(clist[count], count);
		gtk_container_add(GTK_CONTAINER(scrolled_window), clist[count]);				
		
		/*点击notebook页面的clist的标签即会对该列数据进行排序*/
		gtk_clist_column_titles_active(GTK_CLIST(clist[count]));
		gtk_signal_connect(GTK_OBJECT(clist[count]), "click_column", GTK_SIGNAL_FUNC(SortByColumn), NULL);
		
		gtk_signal_connect(GTK_OBJECT(clist[count]), "select_row",GTK_SIGNAL_FUNC(SelectionMade),NULL) ;
		gtk_signal_connect(GTK_OBJECT(clist[count]), "unselect_row",GTK_SIGNAL_FUNC(UnselectionMade),NULL) ;
	}
	g_signal_connect_after(G_OBJECT(notebook), "switch-page", G_CALLBACK(PageSelected), NULL); 	
	
	/*状态栏*/
	statusbar = gtk_statusbar_new();
	gtk_box_pack_start(GTK_BOX(v_box), statusbar, FALSE, FALSE, 0);		
		
	gtk_widget_show_all(window);
}


void SetColumnWidth(GtkWidget *clist, gint page_id)	/*设置标签宽度*/		
{
	switch(page_id)
	{
		case SITE_PAGE:		/*设置网站页面*/
			gtk_clist_set_column_width(GTK_CLIST(clist), 0, 100);
			gtk_clist_set_column_width(GTK_CLIST(clist), 1, 200);
			gtk_clist_set_column_width(GTK_CLIST(clist), 2, 300);
			break;
		case SHOP_PAGE:		/*设置店铺页面*/
			gtk_clist_set_column_width(GTK_CLIST(clist), 0, 90);
			gtk_clist_set_column_width(GTK_CLIST(clist), 1, 70);
			gtk_clist_set_column_width(GTK_CLIST(clist), 2, 100);
			gtk_clist_set_column_width(GTK_CLIST(clist), 3, 100);
			gtk_clist_set_column_width(GTK_CLIST(clist), 4, 200);
			gtk_clist_set_column_width(GTK_CLIST(clist), 5, 100);
			break;
		case DEAL_PAGE:		/*设置交易页面*/
			gtk_clist_set_column_width(GTK_CLIST(clist), 0, 90);
			gtk_clist_set_column_width(GTK_CLIST(clist), 1, 90);
			gtk_clist_set_column_width(GTK_CLIST(clist), 2, 90);
			gtk_clist_set_column_width(GTK_CLIST(clist), 3, 100);
			gtk_clist_set_column_width(GTK_CLIST(clist), 4, 150);
			break;
		case STATI_PAGE:	/*设置统计页面*/
			gtk_clist_set_column_width(GTK_CLIST(clist), 0, 150);
			gtk_clist_set_column_width(GTK_CLIST(clist), 1, 150);
			gtk_clist_set_column_width(GTK_CLIST(clist), 2, 150);
			gtk_clist_set_column_width(GTK_CLIST(clist), 3, 150);
			break;			
		default:
			g_message("Impossible!!");
			break;
	}
}

void SelectionMade(GtkWidget *clist,gint row,gint column,	GdkEventButton *event, gpointer data)		/* 用户选中某一行时的回调函数*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));		/*获取当前页面*/
	
	if (current_page == SITE_PAGE)
		site_selected_row = row;	
	else if (current_page == SHOP_PAGE)
		 	shop_selected_row = row;	
		 else
		 	deal_selected_row = row;
	
	g_message("You have selected row %d in page %d.", row, current_page);
}

gboolean PageSelected(GtkNotebook *notebook, gint arg, gpointer data) 	/*用户选中某一标签页的回调函数*/
{ 
	static gint former_page = SITE_PAGE;		/*上一次选择的页面*/
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));	/*当前页面*/
	
	HandleSwitchPage(current_page, former_page);
	
	former_page = current_page;
	return FALSE; 
} 

/*用户点击CList里某一列调用的回调函数*/
void SortByColumn(GtkCList *clist, gint column)
{
	gtk_clist_set_sort_column(GTK_CLIST(clist), column);
	gtk_clist_sort(GTK_CLIST(clist));	
}

void UnselectionMade(GtkWidget *clist,gint row,gint column,	GdkEventButton *event,	gpointer data)		/* 用户取消选中某一行时的回调函数*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	
	if (current_page == SITE_PAGE)
		site_selected_row = -1;	
	else if (current_page == SHOP_PAGE)
		 	shop_selected_row = -1;	
		 else
		 	deal_selected_row = -1;
}

/**************************************************************************************************/
/***************************************文件菜单栏对应的回调函数**************************************/
/**************************************************************************************************/
void OnFileNewActivate(void)		/*选择"文件/新建"*/
{
	FileNew();					
}
void OnFileOpenActivate(void)		/*选择"文件/打开"*/
{
	FileOpen();
}
void OnFileSaveActivate(void)		/*选择"文件/保存"*/
{
	FileSave();
}
void OnFileSaveasActivate(void)		/*选择"文件/另存为"*/
{
	FileSaveas();
}
void OnFileExitActivate(void)		/*选择"文件/退出"*/
{
	FileExit();
}
void OnExpandAllActivate(void)		/*选择"文件/更多/展开全部"*/
{
	ExpandAll();
}
void OnRetractOthersActivate(void)	/*选择"文件/更多/收起其他"*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	
	RetractOthers(current_page);
}
void OnCheckDataActivate(void)		/*选择"文件/更多/数据排错"*/
{
	CheckData();
}

/**************************************************************************************************/
/***************************************编辑菜单栏对应的回调函数***************************************/
/*************************************************************************************************/
gint GetSelectedRow(gint current_page)		/*返回相应页面所选中的行数*/
{
	switch (current_page)
	{
		case SITE_PAGE:
			return site_selected_row;
		case SHOP_PAGE:
			return shop_selected_row;
		case DEAL_PAGE:
			return deal_selected_row;
		default:
			return -1;
	}
}
void OnAddInfoActivate(void)			/*选择"编辑/添加信息"*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	if (current_page != STATI_PAGE)
		AddInfo(current_page);
}

void OnSearchInfoActivate(void)			/*选择"编辑/查找信息"*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	if (current_page != STATI_PAGE)	
		SearchInfo(current_page);
}

void OnEditInfoActivate(void)			/*选择"编辑/修改信息"*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	gint selected_row = GetSelectedRow(current_page);
	if (current_page != STATI_PAGE)
	{
		if (selected_row != -1)
			EditInfo(current_page, selected_row);
		else
			ShowMessageBox("未选中任何项!");
	}
	
}

void OnDelInfoActivate(void)			/*选择"编辑/删除信息"*/
{
	gint current_page = gtk_notebook_get_current_page(GTK_NOTEBOOK(notebook));
	gint selected_row = GetSelectedRow(current_page);
	if (current_page != STATI_PAGE)
	{
		if (selected_row != -1)
			DelInfo(current_page, selected_row);
		else	
			ShowMessageBox("未选中任何项!");
	}
		
}

/************************************************************************************************/
/***************************************统计菜单栏需要调用的函数*************************************/
/***********************************************************************************************/
void OnStatiSiteActivate(void)		/*选择"统计/网站排行榜"*/
{
	StatiInfo(0);
}
void OnStatiShopActivate(void)		/*选择"统计/店铺交易信息"*/
{
	StatiInfo(1);
}

void OnStatiDealActivate(void)		/*选择"统计/地区贸易情况"*/
{
	StatiInfo(2);
}
void CreateStatiNotebook(gint id)	/*创建“统计结果”页面*/
{
	g_message("CreateStatiNotebook");
	GtkWidget *scrolled_window;
	GtkWidget *label;
	gchar *clist_titles[3][5] = {
		{"名次", "网站编号", "网站名称", "月份", "交易次数"}, 
		{"店铺编号", "店铺名称", "年份", "交易次数", "交易金额"}, 
		{"地区名称", "月份", "资金流入", "资金流出", "贸易情况"}
	};
	gint clist_column = 5;	
	static gboolean first = TRUE;
	
	if (first)	/*如果是第一次建立“统计结果”页面*/
	{
		/*建立滚动窗口*/
		scrolled_window = gtk_scrolled_window_new(NULL, NULL);
		gtk_scrolled_window_set_policy(GTK_SCROLLED_WINDOW(scrolled_window),
				GTK_POLICY_AUTOMATIC, GTK_POLICY_ALWAYS);
		label = gtk_label_new("统计结果");
		gtk_notebook_append_page(GTK_NOTEBOOK(notebook), scrolled_window, label);	
		gtk_container_set_border_width(GTK_CONTAINER(scrolled_window),5);
		
		/*建立GtkCList*/
		clist[3] = gtk_clist_new_with_titles(clist_column, clist_titles[id]);		
		/*设置边框阴影*/
		gtk_clist_set_shadow_type(GTK_CLIST(clist[3]), GTK_SHADOW_OUT);
		/*设置宽度*/
		SetColumnWidth(clist[3], STATI_PAGE);
		gtk_container_add(GTK_CONTAINER(scrolled_window), clist[3]);		
			
		/*为一些信息建立回呼函数*/
		gtk_clist_column_titles_active(GTK_CLIST(clist[3]));
		gtk_signal_connect(GTK_OBJECT(clist[3]), "click_column", GTK_SIGNAL_FUNC(SortByColumn), NULL);
		
		gtk_signal_connect(GTK_OBJECT(clist[3]), "select_row",GTK_SIGNAL_FUNC(SelectionMade),NULL) ;
		gtk_signal_connect(GTK_OBJECT(clist[3]), "unselect_row",GTK_SIGNAL_FUNC(UnselectionMade),NULL) ;
	
		gtk_widget_show_all(notebook);
		gtk_notebook_set_current_page(GTK_NOTEBOOK(notebook), STATI_PAGE);
		first = FALSE;
	}
	else	/*前面已经建立了“统计结果”页面*/
	{
		gtk_notebook_remove_page(GTK_NOTEBOOK(notebook), STATI_PAGE);
		first = TRUE;
		CreateStatiNotebook(id);
	}
}

/**************************************************************************************************/
/*******************************************帮助菜单栏需要调用的函数***********************************/
/*************************************************************************************************/
void OnHelpActivate(void) 		/*选择"帮助/操作说明"*/
{
	gchar *message = "1、第一次运行程序请先依次添加网站、店铺、交易信息。\n\n\
2、编辑菜单栏及工具栏对不同页面会执行相应信息的相关操作。\n\n\
3、菜单\"/文件/更多/展开全部\"即将所有信息输出；\"/文件/更多/收起其他\"即只查看某网站、店铺对应的下级信息，选择后即为\"收起其他\"模式。\n\n\
4、在\"收起其他模式下\",在网站页面下选择某一网站后再切换到店铺页面即可查看，同理可查看某个店铺的交易信息。\
如欲查看某网站的所有交易信息，请先在网站信息页面中选择该网站，再切换到交易信息页面即可查看\n\n\
5、菜单\"/文件/更多/数据排错\"会检查并修正当前数据中可能存在的一些错误(主要检查编号以及店铺个数、交易次数)。\
注意：此功能将重新对店铺及交易进行编号。(删除操作也会执行类似操作。)\n\n\
6、查找只能找到符合的第一项，如要查看后面几项，可先点击对应标签进行排序。\n\n\
7、程序有一不足：点击标签进行排序后如欲修改或删除信息须重新选择对象，否则可能导致结果与自己预想的不一样。\n\n\
7、工具栏可随意拖动。";
	ShowMessageBox(message);
}
void OnAboutActivate(void)		/*选择"帮助/关于本程序"*/
{
	GtkWidget *about;
	const gchar *title = "购物网站信息管理系统";
	const gchar *version = "1.0";
	const gchar *copyright = "Copyright © 2010 Huang Zhigeng";
	const char *authors[] = { "Huang Zhigeng", "\n", "参考资料:	《Linux应用程序开发指南》", "		《GTK编程范例》",
	 "		《开发Liunx应用》",NULL}; 
	
	about = gnome_about_new(title, version, copyright, NULL, authors, NULL, NULL, NULL);
	gtk_widget_show(about);
}

/**************************************************************************************************/
/************************************************辅助函数*＊****************************************/
/*************************************************************************************************/
void ShowMessageBox(gchar *message)		/*弹出内容为message的消息框*/
{
	GtkWidget *message_box;

	message_box = gnome_message_box_new(message, GNOME_MESSAGE_BOX_INFO, GNOME_STOCK_BUTTON_OK, NULL);
	gnome_dialog_set_parent((GnomeDialog *)message_box , (GtkWindow *)window);
	gtk_widget_show(message_box);
}

