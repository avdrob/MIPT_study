{************************************}
{ DECODERC for KBDHOOK, version 1.0. }
{ Copyright (C) 1997 by CodeMaster.  }
{ Fidonet: 2:5002/39.                }
{ E-mail: cm@liceum.secna.ru.        }
{************************************}

program DecoderC;

const
  InFileName = 'C:\DOS\EGA4.CPI';
  OutFileName = 'DECODED';
  XorCode = $4B48;

var
  F: file of Word;
  T: Text;
  W: Word;
  C: Char absolute W;
  S: String;

  procedure Fatal(const Msg: String);
  begin
    WriteLn(Msg);
    Halt
  end;

begin
  WriteLn('DECODERC, version 1.0.');
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
    W := W xor XorCode;
    case C of
      #8: if S[0] <> #0 then Dec(S[0]);
      #13 :
        begin
          WriteLn(T, S);
          S := ''
        end;
      #0..#31: Continue
    else S := S+C
    end
  end;
  if S <> '' then WriteLn(T, S);
  Close(T);
  Close(F);
  WriteLn('OK.')
end.
