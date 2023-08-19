import os
import sys

def create_links(src, dest):
    if not os.path.isdir(src):
        print(f"Error: {src} is not a directory.")
        sys.exit(1)

    if os.path.exists(dest):
        print(f"Error: {dest} already exists.")
        sys.exit(1)

    os.mkdir(dest)

    links_created = {}

    for root, _, files in os.walk(src):
        for file in files:
            src_path = os.path.join(root, file)
            base_name = os.path.basename(file)

            if os.path.islink(src_path):
                continue

            if base_name in links_created:
                link_name = f"{base_name}.{links_created[base_name]}"
                links_created[base_name] += 1
            else:
                link_name = base_name
                links_created[base_name] = 1

            dest_dir = os.path.join(dest, link_name[0].lower())
            os.makedirs(dest_dir, exist_ok=True)

            link_path = os.path.join(dest_dir, link_name)
            os.link(src_path, link_path)
            print(f"{src_path} {link_path}")

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: python script.py <src> <dest>")
        sys.exit(1)

    src = sys.argv[1]
    dest = sys.argv[2]

    create_links(src, dest)
