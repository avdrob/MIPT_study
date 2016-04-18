{*******************************************************}
{ Macro Assembler filter for TASMED.                    }
{ Copyright (C) 1996-99 by Eugene Nonko aka CodeMaster. }
{*******************************************************}

{ Output format:
  <message type> [<filename>]([<line number>[,<column number>]]) <message>
  Where message type is "**Error**", "**Fatal**", "*Warning*" or "*Message*" }

program MAsm_Filter;

const
  strIFatal = 'fatal error';
  strIError = 'error';

  strError = '**Error**';
  strFatal = '**Fatal**';
  strWarning = '*Warning*';

var
  S, MsgType, FileName: string;

begin
  while not EOF do
  begin
    ReadLn(S);
    if S[1] <> ' ' then
    begin
      FileName := Copy(S, 1, Pos(': ', S)-1);
      Delete(S, 1, Length(FileName)+2);
      if Copy(FileName, 1, Length(strIFatal)) = strIFatal then
      begin
        S[1] := UpCase(S[1]);
        S := strFatal+' () '+S
      end
      else
      begin
        if Copy(S, 1, Length(strIError)) = strIError then MsgType := strError else MsgType := strWarning;
        Delete(S, 1, Pos(':', S)+1);
        S[1] := UpCase(S[1]);
        S := MsgType+' '+FileName+' '+S
      end;
      WriteLn(S)
    end
  end
end.
