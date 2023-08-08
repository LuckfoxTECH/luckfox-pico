# Rockchip sample Makefile

.PHONY : clean

all:$(COMM_OBJ)
	@mkdir -p $(SAMPLE_OUT_DIR)/lib
	@$(SAMPLE_AR) crs $(SAMPLE_OUT_DIR)/lib/libsample_comm.a $(COMM_OBJ)

$(COMM_OBJ): $(COMM_SRC)
	@$(SAMPLE_CC) -c $(patsubst %.o,%.c,$@)  -o $@ $(SAMPLE_CFLAGS) $(LD_FLAGS)

clean:
	@make -C isp2.x clean
	@make -C isp3.x clean

