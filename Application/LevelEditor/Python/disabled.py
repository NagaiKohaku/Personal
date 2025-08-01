import bpy
from .add_disabled import MYADDON_OT_add_disabled

#パネル コライダー
class OBJECT_PT_disabled(bpy.types.Panel):
    bl_idname = "OBJECT_PT_disabled"
    bl_label = "Disable"
    bl_space_type = "PROPERTIES"
    bl_region_type = "WINDOW"
    bl_context = "object"

    #サブメニューの描画
    def draw(self,context):

        #パネルに項目を追加
        if "disable" in context.object:

            #すでにプロパティがあれば、プロパティを表示
            self.layout.prop(context.object,'["disable"]',text=self.bl_label)
            
        else:

            #プロパティが無ければ、プロパティ追加ボタンを表示
            self.layout.operator(MYADDON_OT_add_disabled.bl_idname)
