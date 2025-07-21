import bpy

#オペレータ カスタムプロパティ['disable']追加
class MYADDON_OT_add_disabled(bpy.types.Operator):
    bl_idname = "myaddon.myaddon_ot_add_disabled"
    bl_label = "無効フラグ 追加"
    bl_description = "['disable']カスタムプロパティを追加します"
    bl_option = {"REGISTER","UNDO"}

    def execute(self,context):

        #['disable']カスタムプロパティを追加
        context.object["disable"] = True

        return {"FINISHED"}
