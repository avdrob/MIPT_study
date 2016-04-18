{*******************************************************}
{ Microsoft Linker filter for TASMED.                   }
{ Copyright (C) 1996-99 by Eugene Nonko aka CodeMaster. }
{*******************************************************}

{ Output format:
  <message type> [<filename>]([<line number>[,<column number>]]) <message>
  Where message type is "**Error**", "**Fatal**", "*Warning*" or "*Message*" }

program Link_Filter;

const
  strIFatal = 'fatal';
  strIError = 'error';
  strLink = 'LINK ';

  strError = '**Error**';
  strFatal = '**Fatal**';
  strWarning = '*Warning*';

var
  S, MsgType, S1: string;

begin
  while not EOF do
  begin
    ReadLn(S);
    if S[1] <> ' ' then
    begin
      S1 := Copy(S, 1, Pos(':', S)-1);
      if S1 <> strLink then Continue;
      Delete(S, 1, Length(S1)+2);
      if Copy(S, 1, Length(strIFatal)) = strIFatal then MsgType := strFatal else
        if Copy(S, 1, Length(strIError)) = strIError then MsgType := strError else MsgType := strWarning;
      Delete(S, 1, Pos(':', S)+1);
      S[1] := UpCase(S[1]);
      S := MsgType+' () '+S;
      WriteLn(S)
    end
  end
end.
