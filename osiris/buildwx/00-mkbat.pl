use strict 'vars';
my $list = <<EOF;
build\\msw\\wx_adv.vcxproj
build\\msw\\wx_aui.vcxproj
build\\msw\\wx_base.vcxproj
build\\msw\\wx_core.vcxproj
build\\msw\\wx_dbgrid.vcxproj
build\\msw\\wx_gl.vcxproj
build\\msw\\wx_html.vcxproj
build\\msw\\wx_media.vcxproj
build\\msw\\wx_net.vcxproj
build\\msw\\wx_odbc.vcxproj
build\\msw\\wx_qa.vcxproj
build\\msw\\wx_richtext.vcxproj
build\\msw\\wx_wxexpat.vcxproj
build\\msw\\wx_wxjpeg.vcxproj
build\\msw\\wx_wxpng.vcxproj
build\\msw\\wx_wxregex.vcxproj
build\\msw\\wx_wxtiff.vcxproj
build\\msw\\wx_wxzlib.vcxproj
build\\msw\\wx_xml.vcxproj
build\\msw\\wx_xrc.vcxproj
samples\\animate\\anitest.vcxproj
samples\\artprov\\artprov.vcxproj
samples\\aui\\auidemo.vcxproj
samples\\calendar\\calendar.vcxproj
samples\\caret\\caret.vcxproj
samples\\checklst\\checklst.vcxproj
samples\\collpane\\collpane.vcxproj
samples\\config\\config.vcxproj
samples\\controls\\controls.vcxproj
samples\\dataview\\dataview.vcxproj
samples\\db\\db.vcxproj
samples\\dialogs\\dialogs.vcxproj
samples\\dialup\\dialup.vcxproj
samples\\dnd\\dnd.vcxproj
samples\\docview\\docview.vcxproj
samples\\docvwmdi\\docvwmdi.vcxproj
samples\\dragimag\\dragimag.vcxproj
samples\\drawing\\drawing.vcxproj
samples\\dynamic\\dynamic.vcxproj
samples\\erase\\erase.vcxproj
samples\\event\\event.vcxproj
samples\\exec\\exec.vcxproj
samples\\font\\font.vcxproj
samples\\grid\\grid.vcxproj
samples\\help\\help.vcxproj
samples\\html\\about\\about.vcxproj
samples\\html\\helpview\\helpview.vcxproj
samples\\html\\printing\\printing.vcxproj
samples\\html\\test\\test.vcxproj
samples\\html\\virtual\\virtual.vcxproj
samples\\html\\widget\\widget.vcxproj
samples\\html\\zip\\zip.vcxproj
samples\\image\\image.vcxproj
samples\\internat\\internat.vcxproj
samples\\ipc\\ipc_ipcclient.vcxproj
samples\\ipc\\ipc_ipcserver.vcxproj
samples\\joytest\\joytest.vcxproj
samples\\layout\\layout.vcxproj
samples\\listctrl\\listctrl.vcxproj
samples\\mdi\\mdi.vcxproj
samples\\mediaplayer\\mediaplayer.vcxproj
samples\\memcheck\\memcheck.vcxproj
samples\\menu\\menu.vcxproj
samples\\mfc\\mfc.vcxproj
samples\\minifram\\minifram.vcxproj
samples\\minimal\\minimal.vcxproj
samples\\nativdlg\\nativdlg.vcxproj
samples\\notebook\\notebook.vcxproj
samples\\oleauto\\oleauto.vcxproj
samples\\opengl\\cube\\cube.vcxproj
samples\\opengl\\isosurf\\isosurf.vcxproj
samples\\opengl\\penguin\\penguin.vcxproj
samples\\ownerdrw\\ownerdrw.vcxproj
samples\\png\\png.vcxproj
samples\\popup\\popup.vcxproj
samples\\propsize\\propsize.vcxproj
samples\\regtest\\regtest.vcxproj
samples\\richtext\\richtext.vcxproj
samples\\rotate\\rotate.vcxproj
samples\\sashtest\\sashtest.vcxproj
samples\\scroll\\scroll.vcxproj
samples\\scrollsub\\scrollsub.vcxproj
samples\\sockets\\sockets_client.vcxproj
samples\\sockets\\sockets_server.vcxproj
samples\\splitter\\splitter.vcxproj
samples\\statbar\\statbar.vcxproj
samples\\taskbar\\taskbar.vcxproj
samples\\text\\text.vcxproj
samples\\thread\\thread.vcxproj
samples\\toolbar\\toolbar.vcxproj
samples\\treectrl\\treectrl.vcxproj
samples\\typetest\\typetest.vcxproj
samples\\validate\\validate.vcxproj
samples\\widgets\\widgets.vcxproj
samples\\wizard\\wizard.vcxproj
samples\\xrc\\xrcdemo.vcxproj
build\\msw\\wx.sln
samples\\samples.sln
EOF

my @aList = split /[\r\n]+/,$list;
my $base;
open FOUT1,">00-copyAll.bat";
open FOUT2,">00-backupAll.bat";
for my $file (@aList)
{
	$base = $file;
	$base =~ s/.*\\//g;
	print FOUT1 "copy /-Y \"${base}\" \"%WXHOME\%\\${file}\"\n";
	print FOUT2 "copy /-Y \"%WXHOME\%\\${file}\" .\n";
}
close FOUT1;
close FOUT2;
