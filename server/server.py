import os
import signal
import subprocess
import sys

from watchfiles import watch, DefaultFilter

def start(script_name: str) -> subprocess.Popen[bytes]:
    if sys.platform == "win32":
        return subprocess.Popen([sys.executable, script_name], creationflags=subprocess.CREATE_NEW_PROCESS_GROUP)

    return subprocess.Popen([sys.executable, script_name], **kwargs)

def finish(process: subprocess.Popen[bytes]) -> None:
    if sys.platform == "win32":
        process.send_signal(signal.CTRL_BREAK_EVENT)
    else:
        process.send_signal(signal.SIGINT)

def main():
    target_script = "src/main.py"
    custom_filter = DefaultFilter(
        ignore_paths=[
            '.git', 
            '__pycache__', 
            '.pytest_cache'
        ],
        ignore_entity_patterns=[
            r'.*\.log$',
            r'.*\.tmp$' 
        ]
    )

    print(f"[Watcher] Listening for file changes...")
    print(f"[Watcher] Starting Server...\n")
    current_process = start(target_script)
    
    try:
        for changes in watch('src', watch_filter=custom_filter):
            for change_type, file_path in changes:
                relative_path = os.path.relpath(file_path, start='src')
                print(f"\n[Watcher] Detected {relative_path} was {change_type.name}")
            print("[Watcher] Restarting app...\n")
            
            finish(current_process) 
            try:
                current_process.wait(timeout=3)
            except subprocess.TimeoutExpired:
                current_process.kill()
            
            print()
            current_process = start(target_script)

    except KeyboardInterrupt:
        print("\n[Watcher] Shutting down...\n")
        finish(current_process)
        current_process.wait()

if __name__ == "__main__":
    main()
