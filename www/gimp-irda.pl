#!/usr/bin/perl


use Gimp ":auto";
use Gimp::Fu;

sub webmenu() {
  my ($words, $indent, $file, $stdcolor, $selcolor, $ovrcolor, $width, $height) =@_;
  
  @wordlist=split(",",$words);
  $shadow_x=2;
  $shadow_y=2;
  $shadow_blur=3;
  $shadow_opac=40;
  $shadow_color="#000000";

  $fontsize=12;
  $x=$indent;
  $baseline0=$height-5; # base line for font
  $baseline1=$height-3; # base line for blue stripe
  
  $nr=0;
  foreach $word  (@wordlist) 
  {
  $count=0;
  foreach $color (($stdcolor,$selcolor,$ovrcolor))
  {
  $img=gimp_image_new($width,$height,RGB);
  gimp_undo_push_group_start($img);

  $lay_back=gimp_layer_new($img,$width,$height,RGB,"Background",100,NORMAL_MODE);
  gimp_image_add_layer($img,$lay_back,-1);
  $dr_back=gimp_image_active_drawable($img);
  gimp_palette_set_background("#ffe496");
  gimp_drawable_fill($dr_back,BG_IMAGE_FILL);
  # build blue line
  #gimp_rect_select($img,$x,$baseline1-1,$width-10-$x,1,REPLACE,0,0);
  #gimp_palette_set_background("#2222ee");
  #gimp_edit_fill($dr_back,BG_IMAGE_FILL);
  
  # draw the text
  gimp_palette_set_foreground($color);
  $lay_text=gimp_text($img,-1,0,0,$word,-1,1,$fontsize,PIXELS,
             "monotype","arial","bold","r","*","*","iso8859","1");
  $dr_text=gimp_image_active_drawable($img);
  $dr_w=gimp_drawable_width($dr_text);
  $dr_h=gimp_drawable_height($dr_text);
  $y=$baseline0-$dr_h;
  gimp_layer_set_offsets($lay_text,$x,$y);

  # put the shadow
  gimp_layer_add_alpha($dr_text);
  gimp_selection_layer_alpha($dr_text);
  my($sel_stat,$sel_x1,$sel_y1,$sel_x2,$sel_y2) = gimp_selection_bounds($img);
  $shadow_w=($sel_x2-$sel_x1)+2*$shadow_blur;
  $shadow_h=($sel_y2-$sel_y1)+2*$shadow_blur;
  #$shadow_x=$shadow_x-$shadow_blur;
  #$shadow_y=$shadow_y-$shadow_blur;
  
  $lay_shadow=gimp_layer_new($img,$shadow_w,$shadow_h,RGBA_IMAGE,
              "Text Shadow",$shadow_opac,NORMAL_MODE);
  gimp_image_add_layer($img,$lay_shadow,-1);
  gimp_layer_set_offsets($lay_shadow,$x,$y);
  gimp_drawable_fill($lay_shadow,TRANS_IMAGE_FILL);
  gimp_palette_set_background($shadow_color);
  gimp_edit_fill($lay_shadow,BG_IMAGE_FILL);
  gimp_selection_none($img);
  
  gimp_layer_set_preserve_trans($lay_shadow,0);
  plug_in_gauss_rle($lay_shadow,$shadow_blur,1,1);
  gimp_layer_translate($lay_shadow,$shadow_x,$shadow_y);
  
  gimp_image_set_active_layer($lay_text);
  

  

  # guide lines
  #$hguide0 = gimp_image_add_hguide($img,$baseline0);
  #$hguide1 = gimp_image_add_hguide($img,$baseline1);
  #$vguide0 = gimp_image_add_vguide($img,$x);
  
  # combin layer-ele
  if ($count==0) { $type="std"; }
  if ($count==1) { $type="sel"; }
  if ($count==2) { $type="ovr"; }
  
  $lay_final=gimp_image_merge_visible_layers($img,CLIP_TO_IMAGE);
  $drawable=gimp_image_active_drawable($img);
#  file_png_save(RUN_NONINTERACTIVE,$img,$drawable,"$file$nr-$type.png","",1,9,1,0,0,1,1);
  gimp_convert_indexed($img,0,0,255,0,0,"");
  file_gif_save(RUN_NONINTERACTIVE,$img,$drawable,"$file$nr-$type.gif","",0,0,0,0);
  gimp_undo_push_group_end($img);  

  print "$file$nr-$type.png\n";

  $count++;
  }
  $nr++;
  }
  
  return undef;
}


register 
  "webmenu",
  "Web Menus",
  "Make buttons",
  "Claudiu Costin",
  "Claudiu Costin (c)",
  "2001-09-05",
  "<Toolbox>/Xtns/Perl-Fu/Claudiu/Web Menu",
  "*",
  [
   [PF_STRING, "menus", "Menu items list (words separated by commas)", ""],
   [PF_INT, "identation", "Text indentation", 10],
   [PF_STRING, "filename", "File base name for generated files", "menu"],
   [PF_COLOR, "normal", "Menu normal color", "#000000" ],
   [PF_COLOR, "selected", "Menu selected color", "#fb2020" ],
   [PF_COLOR, "hover", "Menu hover color", "#2066ff" ],
   [PF_INT, "width", "Menu width", 100 ],
   [PF_INT, "height", "Menu height", 18 ],
  ],
  \&webmenu;

exit main();

