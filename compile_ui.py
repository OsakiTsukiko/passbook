ui_input = open('ui.html', 'r')
ui_lines: list[str] = ui_input.readlines()

output_lines: list[str] = []

output_lines.append("static const char UI[] =");

for line in ui_lines:
    l = line.replace("\"", "\\\"")
    l = l.replace("\n", "")
    l = "\"" + l + "\"\n"
    output_lines.append(l)

output_lines.append(";");

ui_output = open('src/ui.h', 'w')
ui_output.writelines(output_lines)
ui_output.close()