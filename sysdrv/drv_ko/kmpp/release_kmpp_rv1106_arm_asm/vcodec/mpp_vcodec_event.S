	.arch armv7-a
	.eabi_attribute 20, 1
	.eabi_attribute 21, 1
	.eabi_attribute 23, 3
	.eabi_attribute 24, 1
	.eabi_attribute 25, 1
	.eabi_attribute 26, 2
	.eabi_attribute 30, 4
	.eabi_attribute 34, 1
	.eabi_attribute 18, 2
	.file	"mpp_vcodec_event.c"
	.text
	.syntax unified
	.syntax unified
	.thumb
	.syntax unified
	.align	1
	.global	mpp_vcodec_dec_event_notify
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_dec_event_notify, %function
mpp_vcodec_dec_event_notify:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_dec_event_notify, .-mpp_vcodec_dec_event_notify
	.align	1
	.global	mpp_vcodec_enc_event_notify
	.syntax unified
	.thumb
	.thumb_func
	.fpu softvfp
	.type	mpp_vcodec_enc_event_notify, %function
mpp_vcodec_enc_event_notify:
	.fnstart
	@ args = 0, pretend = 0, frame = 0
	@ frame_needed = 0, uses_anonymous_args = 0
	@ link register save eliminated.
	movs	r0, #0
	bx	lr
	.fnend
	.size	mpp_vcodec_enc_event_notify, .-mpp_vcodec_enc_event_notify
	.global	vcodec_msg_enc_out_block
	.global	vcodec_msg_enc_out_strm_end
	.global	vcodec_msg_enc_out_strm_buf_rdy
	.global	vcodec_msg_enc_out_strm_q_full
	.global	vcodec_msg_enc_in_block
	.global	vcodec_msg_enc_in_frm_end
	.global	vcodec_msg_enc_in_frm_early_end
	.global	vcodec_msg_enc_in_frm_start
	.global	vcodec_msg_enc_in_frm_cfg_rdy
	.global	vcodec_msg_chan_resume
	.global	vcodec_msg_chan_pause
	.global	vcodec_msg_chan_stop
	.global	vcodec_msg_chan_start
	.global	vcodec_msg_chan_control
	.global	vcodec_msg_chan_reset
	.global	vcodec_msg_chan_destroy
	.global	vcodec_msg_chan_create
	.bss
	.align	3
	.type	vcodec_msg_enc_out_block, %object
	.size	vcodec_msg_enc_out_block, 32
vcodec_msg_enc_out_block:
	.space	32
	.type	vcodec_msg_enc_out_strm_end, %object
	.size	vcodec_msg_enc_out_strm_end, 32
vcodec_msg_enc_out_strm_end:
	.space	32
	.type	vcodec_msg_enc_out_strm_buf_rdy, %object
	.size	vcodec_msg_enc_out_strm_buf_rdy, 32
vcodec_msg_enc_out_strm_buf_rdy:
	.space	32
	.type	vcodec_msg_enc_out_strm_q_full, %object
	.size	vcodec_msg_enc_out_strm_q_full, 32
vcodec_msg_enc_out_strm_q_full:
	.space	32
	.type	vcodec_msg_enc_in_block, %object
	.size	vcodec_msg_enc_in_block, 32
vcodec_msg_enc_in_block:
	.space	32
	.type	vcodec_msg_enc_in_frm_end, %object
	.size	vcodec_msg_enc_in_frm_end, 32
vcodec_msg_enc_in_frm_end:
	.space	32
	.type	vcodec_msg_enc_in_frm_early_end, %object
	.size	vcodec_msg_enc_in_frm_early_end, 32
vcodec_msg_enc_in_frm_early_end:
	.space	32
	.type	vcodec_msg_enc_in_frm_start, %object
	.size	vcodec_msg_enc_in_frm_start, 32
vcodec_msg_enc_in_frm_start:
	.space	32
	.type	vcodec_msg_enc_in_frm_cfg_rdy, %object
	.size	vcodec_msg_enc_in_frm_cfg_rdy, 32
vcodec_msg_enc_in_frm_cfg_rdy:
	.space	32
	.type	vcodec_msg_chan_resume, %object
	.size	vcodec_msg_chan_resume, 32
vcodec_msg_chan_resume:
	.space	32
	.type	vcodec_msg_chan_pause, %object
	.size	vcodec_msg_chan_pause, 32
vcodec_msg_chan_pause:
	.space	32
	.type	vcodec_msg_chan_stop, %object
	.size	vcodec_msg_chan_stop, 32
vcodec_msg_chan_stop:
	.space	32
	.type	vcodec_msg_chan_start, %object
	.size	vcodec_msg_chan_start, 32
vcodec_msg_chan_start:
	.space	32
	.type	vcodec_msg_chan_control, %object
	.size	vcodec_msg_chan_control, 40
vcodec_msg_chan_control:
	.space	40
	.type	vcodec_msg_chan_reset, %object
	.size	vcodec_msg_chan_reset, 32
vcodec_msg_chan_reset:
	.space	32
	.type	vcodec_msg_chan_destroy, %object
	.size	vcodec_msg_chan_destroy, 32
vcodec_msg_chan_destroy:
	.space	32
	.type	vcodec_msg_chan_create, %object
	.size	vcodec_msg_chan_create, 40
vcodec_msg_chan_create:
	.space	40
	.ident	"GCC: (crosstool-NG 1.24.0) 8.3.0"
	.section	.note.GNU-stack,"",%progbits
