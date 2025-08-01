import bpy
from .add_objectgroup import MYADDON_OT_add_objectgroup

#パネル ファイル名
class OBJECT_PT_object_group(bpy.types.Panel):
    """オブジェクトのファイル名パネル"""

    bl_idname = "OBJECT_PT_object_group"
    bl_label = "ObjectGroup"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self,context):

        #パネルに項目を追加
        if "object_group" in context.object:

            #すでにプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["object_group"]',text=self.bl_label)
        else:

            #プロパティが無ければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_objectgroup.bl_idname)
