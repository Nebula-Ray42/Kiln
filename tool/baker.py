import bpy
import sys
import os

current_dir = os.path.dirname(os.path.abspath(__file__))
project_root = os.path.dirname(current_dir)

sys.path.append(os.path.join(current_dir, 'libs'))

generated_dir = os.path.join(project_root, 'schema', 'generated')
sys.path.append(generated_dir)

import flatbuffers
import kiln.schema.Mesh as MeshSchema
import kiln.schema.Vertex as VertexSchema

def bake_mesh(output_filepath):
    print(f"\n=== [Kiln Baker] アセットの書き出しを開始: {output_filepath} ===")

    for obj in bpy.context.scene.objects:
        if obj.type == 'MESH':
            print(f"[*] メッシュを確認: {obj.name}")
            mesh = obj.data

            builder = flatbuffers.Builder(1024)

            MeshSchema.StartVerticesVector(builder, len(mesh.vertices))

            for v in reversed(mesh.vertices):
                VertexSchema.CreateVertex(
                    builder,
                    v.co.x, v.co.y, v.co.z,
                    v.normal.x, v.normal.y, v.normal.z,
                    0.0, 0.0
                )

            vertices_offset = builder.EndVector()

            MeshSchema.Start(builder)
            MeshSchema.AddVertices(builder, vertices_offset)
            mesh_offset = MeshSchema.End(builder)

            builder.Finish(mesh_offset)

            with open(output_filepath, "wb") as f:
                f.write(builder.Output())

            print(f"    -> 頂点数: {len(mesh.vertices)} を {output_filepath} にベイクしました")

            break

    print("=== [Kiln Baker] 書き出し完了 ===\n")


if __name__ == "__main__":
    if "--" in sys.argv:
        argv = sys.argv[sys.argv.index("--") + 1:]
        output_path = argv[0] if len(argv) > 0 else "test_output.bin"
    else:
        output_path = "test_output.bin"

    bake_mesh(output_path)