
clang -S -emit-llvm main.c -o - | opt -analyze -dot-callgraph

$(TARGET)_3_65.out: $(TARGET).bc
	../pipair $<callgraph.dot>$@ 2>&1

# $(TARGET)_10_80.out: $(TARGET).bc
# 	../pipair $< 10 80 >$@ 2>&1

outputs: $(TARGET)_3_65.out $(TARGET)_10_80.out

