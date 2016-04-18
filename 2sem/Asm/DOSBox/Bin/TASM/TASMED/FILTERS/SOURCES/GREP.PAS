{*******************************************************}
{ Turbo GREP filter for TASMED.                         }
{ Copyright (C) 1996-99 by Eugene Nonko aka CodeMaster. }
{*******************************************************}

{ Output format:
  <message type> [<filename>]([<line number>[,<column number>]]) <message>
  Where message type is "**Error**", "**Fatal**", "*Warning*" or "*Message*" }

program Grep_Filter;

const
  strFile = 'File ';
  strIError = 'Error:';

  strMessage = '*Message*';
  strFatal = '**Fatal**';

var
  S, FileName, L: string;
  E: Integer;
  LineNum: Word;

begin
  while not EOF do
  begin
    ReadLn(S);
    if Copy(S, 1, Length(strFile)) = strFile then FileName := Copy(S, 6, Length(S)-6) else
      if Copy(S, 1, Length(strIError)) = strIError then
      begin
        Delete(S, 1, Length(strIError));
        WriteLn(strFatal, ' () Command line:', S)
      end
      else
        if FileName <> '' then
        begin
          Val(Copy(S, 1, Pos(' ', S)-1), LineNum, E);
          if E = 0 then
          begin
            Delete(S, 1, 8);
            Str(LineNum, L);
            WriteLn(strMessage, ' ', FileName, '(', L, ') Matching string found')
          end
          else WriteLn(strMessage, ' ', FileName, '() Matching string found')
        end
  end
end.
