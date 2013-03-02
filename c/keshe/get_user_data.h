#ifndef __CALLSBACK_H__
#define __CALLSBACK_H__

void FileNew(void);											/*选择"文件/新建"*/
void FileOpen(void);										/*选择"文件/打开"*/
void FileSave(void);										/*选择"文件/保存"*/
void FileSaveas(void);										/*选择"文件/另存为"*/
void ExpandAll(void);										/*选择"文件/更多/展开全部"*/
void RetractOthers(gint current_page);						/*选择"文件/更多/收起其他"*/
void CheckData(void);										/*选择"文件/更多/数据排错"*/
void FileExit(void);										/*选择"文件/退出"*/

void AddInfo(gint current_page);							/*选择"编辑/添加信息*/
void SearchInfo(gint current_page);							/*选择"编辑/查找信息*/
void EditInfo(gint current_page, gint selected_row);		/*选择"编辑/编辑信息"*/
void DelInfo(gint current_page, gint selected_row);			/*选择"编辑/删除信息"*/

void StatiInfo(gint id);									/*选择统计菜单栏任一项时调用，根据id选择统计哪方面信息*/
	
void HandleSwitchPage(gint current_page, gint former_page);	/*处理切换页面*/
void OnWindowDeleteEvent(GtkNotebook *notebook);			/*退出前调用的函数*/
void OnDataChanged(void);									/*数据发生改变,修改变量file_modifyed使其为TRUE*/
void OnDataSaved(void);										/*数据保存后调用的函数，修改变量file_modifyed使其为FALSE*/


#endif
