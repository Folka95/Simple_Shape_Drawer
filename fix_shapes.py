import os, re

for root, dirs, files in os.walk('core/shapes'):
    for f in files:
        if f.endswith('.cpp'):
            filepath = os.path.join(root, f)
            with open(filepath, 'r') as file:
                content = file.read()
            
            # The pattern is usually ClassName::ClassName() : Shape(SHAPE_TYPE, "...")
            # We want to change it to Shape(SHAPE_TYPE, "ClassName", "...")
            # But the "..." string spans multiple lines.
            
            # Find class name from file name or constructor
            match = re.search(r'(\w+)::\1', content)
            if not match: continue
            class_name = match.group(1)
            
            # We look for Shape(SHAPE_TYPE,
            pattern = r'Shape\s*\(\s*(SHAPE_[A-Z_]+)\s*,'
            if re.search(pattern, content):
                new_content = re.sub(pattern, f'Shape(\\1, "{class_name}",', content)
                with open(filepath, 'w') as file:
                    file.write(new_content)
        elif f.endswith('.h'):
            # Some shapes might have inline constructors (like PolygonShape<size>)
            filepath = os.path.join(root, f)
            with open(filepath, 'r') as file:
                content = file.read()
            if 'PolygonShape' in filepath:
                # PolygonShape has Shape(SHAPE_POLYGON, ...)
                pattern = r'Shape\s*\(\s*(SHAPE_POLYGON)\s*,'
                if re.search(pattern, content):
                    new_content = re.sub(pattern, r'Shape(\1, "Polygon",', content)
                    with open(filepath, 'w') as file:
                        file.write(new_content)
