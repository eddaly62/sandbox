#! /bin/bash
BRANCH=$(git rev-parse --abbrev-ref HEAD)
LOCALSHA=$(git rev-parse HEAD)
BOD=$(date)
echo "// dapID.c" > src/dapID.c
echo "// This an AUTO-GENERATED file, DO NOT EDIT" >> src/dapID.c
echo "// It contains creation data that will be embedded in the dap library" >> src/dapID.c
echo "// Include in dapinit function so it also gets included in the application" >> src/dapID.c
echo -ne "const char dapid[] = \"DAP: Date($BOD), Branch($BRANCH), SHA($LOCALSHA)\";\n" >> src/dapID.c
echo "char *dap_id(void) {" >> src/dapID.c
echo -ne "\treturn (char *)dapid;\n" >> src/dapID.c
echo "}" >> src/dapID.c
