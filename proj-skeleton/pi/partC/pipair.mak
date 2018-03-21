
$(TARGET)_3_65.out: $(TARGET).bc
	../pipair $< 1 >$@ 2>&1

$(TARGET)_10_80.out: $(TARGET).bc
	../pipair $< 1 10 80 >$@ 2>&1

outputs: $(TARGET)_3_65.out $(TARGET)_10_80.out

