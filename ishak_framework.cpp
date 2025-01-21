/*
ishak_framework.cpp - Adds some functionality custom like: search in selection, show buffer path....
@ishak: rememeber to add this .cpp to the 4coder_default_include.cpp file for this to compile and metadata to be generated.
this commands can be bound to the bindings.4coder file under the code category

*/

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
    if (buildBuffer != 0) 
    {       
        for(int i = 0; i < file_path.size; ++i)
        {
            // The intention behind this is to easy copy-paste the header files.
            u8& character = file_path.str[i];            
            if(character == '\\')
            {
                character = '/';
            }                         
        }
        
        
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


CUSTOM_COMMAND_SIG(p4_edit_current_file) 
CUSTOM_DOC("Checking out this file for p4.")
{
    // Get the active buffer
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    
    // Ensure the buffer is file-backed
    
        // Get the file path
        Scratch_Block scratch(app);
        String_Const_u8 file_path = push_buffer_file_name(app, scratch, buffer);
        
        // Construct and execute the `p4 edit` command
    if (file_path.size > 0)
    {
        char modified_path[1024];
        snprintf(modified_path, sizeof(modified_path), "%.*s", string_expand(file_path));
        if (modified_path[0] == 'D') 
        {
            modified_path[0] = 'C'; // Replace 'D' with 'C'
        }
        
        // Construct and execute the `p4 edit` command
        char command[1024];
        snprintf(command, sizeof(command), "p4 edit \"%s\"", modified_path);
        system(command);
            
            // Notify the user
            print_message(app, string_u8_litexpr("Ran: "));
            print_message(app, SCu8(command));
            print_message(app, string_u8_litexpr("\n"));
    }
    else 
    {
            print_message(app, string_u8_litexpr("Error: Unable to determine file path.\n"));
        }
    
}


CUSTOM_COMMAND_SIG(p4_add_current_file) 
CUSTOM_DOC("Adding this file to p4.")
{
    // Get the active buffer
    View_ID view = get_active_view(app, Access_Always);
    Buffer_ID buffer = view_get_buffer(app, view, Access_Always);
    
    // Ensure the buffer is file-backed
    
    // Get the file path
    Scratch_Block scratch(app);
    String_Const_u8 file_path = push_buffer_file_name(app, scratch, buffer);
    
    // Construct and execute the `p4 edit` command
    if (file_path.size > 0)
    {
        char modified_path[1024];
        snprintf(modified_path, sizeof(modified_path), "%.*s", string_expand(file_path));
        if (modified_path[0] == 'D') 
        {
            modified_path[0] = 'C'; // Replace 'D' with 'C'
        }
        
        // Construct and execute the `p4 add` command
        char command[1024];
        snprintf(command, sizeof(command), "p4 add \"%s\"", modified_path);
        system(command);
        
        // Notify the user
        print_message(app, string_u8_litexpr("Ran: "));
        print_message(app, SCu8(command));
        print_message(app, string_u8_litexpr("\n"));
    }
    else 
    {
        print_message(app, string_u8_litexpr("Error: Unable to determine file path.\n"));
    }
    
}
