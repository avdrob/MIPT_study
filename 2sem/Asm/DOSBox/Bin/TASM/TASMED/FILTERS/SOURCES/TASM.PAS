{*******************************************************}
{ Turbo Assembler filter for TASMED.                    }
{ Copyright (C) 1996-99 by Eugene Nonko aka CodeMaster. }
{*******************************************************}

{ Output format:
  <message type> [<filename>]([<line number>[,<column number>]]) <message>
  Where message type is "**Error**", "**Fatal**", "*Warning*" or "*Message*" }

program TAsm_Filter;

const
  strError = '**Error**';
  strFatal = '**Fatal**';
  strWarning = '*Warning*';

var
  S, MsgType, FileName: string;

begin
  while not EOF do
  begin
    ReadLn(S);
    MsgType := Copy(S, 1, 9);
    if (MsgType = strError) or (MsgType = strWarning) or (MsgType = strFatal) then
    begin
      FileName := S;
      Delete(FileName, 1, 11);
      FileName[0] := Char(Pos(' ', FileName)-1);
      if Pos('(', FileName) = 0 then Insert('() ', S, 11);
      WriteLn(S)
    end
  end
end.
