

CUSTOM_COMMAND_SIG(show_buffer_path)
CUSTOM_DOC("Shows the Path of the current Buffer.")
{
    // Get the active view and buffer
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    
    // Get the file path
    Scratch_Block scratch(app);
    String_Const_u8 file_path = push_buffer_file_name(app, scratch, buffer);
    
    View_ID buildPanel = get_or_open_build_panel(app);
    
    // Get the buffer associated with the Build Panel
    Buffer_ID buildBuffer = view_get_buffer(app, buildPanel, Access_Always);
    if (buildBuffer != 0) {
        // Write the file path into the Build Panel buffer
        String_Const_u8 message = push_stringf(scratch, "Current File Path:\n%.*s\n", string_expand(file_path));
        
        // Append the message to the buffer
        i64 buffer_size = buffer_get_size(app, buildBuffer);
        buffer_replace_range(app, buildBuffer, Ii64(buffer_size), message);
    }
}

CUSTOM_COMMAND_SIG(search_in_selection)
CUSTOM_DOC("Performs a seach in the current selection.")
{
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    
    Scratch_Block scratch(app);
    Range_i64 range = get_view_range(app, view);
    String_Const_u8 selected_text = push_buffer_range(app, scratch, buffer, range);
    
    // Check if the selected text is non-empty
    if (selected_text.size == 0) {
        print_message(app, string_u8_litexpr("Cannot search for empty selection.\n"));
        return;
    }
       
    // Call 4coder's built-in isearch function
    isearch(app, Scan_Forward, range.min, selected_text);
    }
