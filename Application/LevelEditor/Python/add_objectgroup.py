import bpy

#オペレータ カスタムプロパティ['object_group']を追加
class MYADDON_OT_add_objectgroup(bpy.types.Operator):

    bl_idname = "myaddon.myaddon_ot_add_objectgroup"
    bl_label = "ObjectGroup 追加"
    bl_description = "['object_group']カスタムプロパティを追加します"
    bl_options = {"REGISTER","UNDO"}

    def execute(self,context):

        #['file_name']カスタムプロパティを追加
        context.object["object_group"] = ""

        return {"FINISHED"}
